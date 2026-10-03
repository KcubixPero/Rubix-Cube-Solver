#include "HttpApi.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

using Socket = SOCKET;

static constexpr Socket INVALID_SOCKET_VALUE = INVALID_SOCKET;

static void closeSocket(Socket socket)
{
    closesocket(socket);
}
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using Socket = int;

static constexpr Socket INVALID_SOCKET_VALUE = -1;

static void closeSocket(Socket socket)
{
    close(socket);
}
#endif

#include "Color.h"
#include "CrossSolver.h"
#include "F2LSolver.h"
#include "MoveParser.h"
#include "OLLSolver.h"
#include "PLLSolver.h"
#include "RubixCube.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
class CubeJsonParser
{
public:
    explicit CubeJsonParser(const std::string& text) : text_(text) {}

    vector3d parseCube()
    {
        const auto key = text_.find("\"cube\"");
        if (key == std::string::npos)
            throw std::invalid_argument("Request must include a cube array.");

        position_ = text_.find(':', key + 6);
        if (position_ == std::string::npos)
            throw std::invalid_argument("Malformed cube field.");

        ++position_;

        vector3d cube;
        expect('[');

        for (int face = 0; face < 6; ++face)
        {
            expect('[');

            vector2d rows;

            for (int row = 0; row < 3; ++row)
            {
                expect('[');

                std::vector<int> cells;

                for (int col = 0; col < 3; ++col)
                {
                    cells.push_back(parseInteger());

                    if (col < 2)
                        expect(',');
                }

                expect(']');
                rows.push_back(std::move(cells));

                if (row < 2)
                    expect(',');
            }

            expect(']');
            cube.push_back(std::move(rows));

            if (face < 5)
                expect(',');
        }

        expect(']');
        return cube;
    }

private:
    void skipWhitespace()
    {
        while (position_ < text_.size() &&
               std::isspace(static_cast<unsigned char>(text_[position_])))
        {
            ++position_;
        }
    }

    void expect(char value)
    {
        skipWhitespace();

        if (position_ >= text_.size() || text_[position_] != value)
            throw std::invalid_argument(
                "Malformed cube array: expected '" + std::string(1, value) + "'.");

        ++position_;
    }

    int parseInteger()
    {
        skipWhitespace();

        std::size_t consumed = 0;
        int value = 0;

        try
        {
            value = std::stoi(text_.substr(position_), &consumed);
        }
        catch (...)
        {
            throw std::invalid_argument("Cube colors must be integers from 0 to 5.");
        }

        position_ += consumed;
        return value;
    }

    const std::string& text_;
    std::size_t position_ = 0;
};

void validateCube(const vector3d& cube)
{
    if (cube.size() != 6)
        throw std::invalid_argument("Cube must contain six faces.");

    int colorCounts[6] = {};

    for (int face = 0; face < 6; ++face)
    {
        if (cube[face].size() != 3)
            throw std::invalid_argument("Each cube face must have three rows.");

        for (const auto& row : cube[face])
        {
            if (row.size() != 3)
                throw std::invalid_argument("Each cube row must have three stickers.");

            for (int color : row)
            {
                if (color < 0 || color > 5)
                    throw std::invalid_argument(
                        "Cube colors must be integers from 0 to 5.");

                ++colorCounts[color];
            }
        }

        if (cube[face][1][1] != face)
            throw std::invalid_argument(
                "Cube centers must match the fixed W/R/B/G/O/Y face orientation.");
    }

    for (int count : colorCounts)
    {
        if (count != 9)
            throw std::invalid_argument(
                "A valid cube must contain nine stickers of each color.");
    }
}

std::string jsonMoves(const std::vector<std::string>& moves)
{
    std::ostringstream json;
    json << '[';

    for (std::size_t i = 0; i < moves.size(); ++i)
    {
        if (i)
            json << ',';

        json << '"' << moves[i] << '"';
    }

    json << ']';
    return json.str();
}

std::string solutionJson(const std::vector<std::string>& cross,
                         const std::vector<std::string>& f2l,
                         const std::vector<std::string>& oll,
                         const std::vector<std::string>& pll)
{
    const std::size_t total =
        cross.size() + f2l.size() + oll.size() + pll.size();

    std::ostringstream json;

    json << "{\"scramble\":[],\"cross\":" << jsonMoves(cross)
         << ",\"f2l\":" << jsonMoves(f2l)
         << ",\"oll\":" << jsonMoves(oll)
         << ",\"pll\":" << jsonMoves(pll)
         << ",\"counts\":{\"cross\":" << cross.size()
         << ",\"f2l\":" << f2l.size()
         << ",\"oll\":" << oll.size()
         << ",\"pll\":" << pll.size()
         << ",\"total\":" << total
         << "},\"crossCount\":" << cross.size()
         << ",\"f2lCount\":" << f2l.size()
         << ",\"ollCount\":" << oll.size()
         << ",\"pllCount\":" << pll.size()
         << ",\"totalCount\":" << total << '}';

    return json.str();
}

std::string jsonEscape(const std::string& value)
{
    std::string escaped;

    for (char ch : value)
    {
        if (ch == '"' || ch == '\\')
            escaped.push_back('\\');

        if (ch == '\n')
        {
            escaped += "\\n";
            continue;
        }

        if (ch == '\r')
        {
            escaped += "\\r";
            continue;
        }

        escaped.push_back(ch);
    }

    return escaped;
}

std::string errorJson(const std::string& message)
{
    return "{\"error\":\"" + jsonEscape(message) + "\"}";
}

std::pair<int, std::string> solveRequest(const std::string& body)
{
    try
    {
        vector3d state = CubeJsonParser(body).parseCube();
        validateCube(state);

        RubixCube cube(state);

        const auto cross = CrossSolver::solve(cube);

        for (const auto& move : cross)
            MoveParser::executeMove(cube, move);

        if (!CrossSolver::isSolved(cube))
            throw std::runtime_error(
                "Cross solver could not solve the white cross.");

        const auto f2l = F2LSolver::solve(cube);

        if (!F2LSolver::isSolved(cube))
            throw std::runtime_error("F2L stage verification failed.");

        const auto oll = OLLSolver::solve(cube);

        if (!OLLSolver::isSolved(cube))
            throw std::runtime_error("OLL stage verification failed.");

        const auto pll = PLLSolver::solve(cube);

        if (!PLLSolver::isSolved(cube))
            throw std::runtime_error("PLL stage verification failed.");

        return {200, solutionJson(cross, f2l, oll, pll)};
    }
    catch (const std::invalid_argument& error)
    {
        return {400, errorJson(error.what())};
    }
    catch (const std::exception& error)
    {
        return {422, errorJson(error.what())};
    }
}

bool sendAll(Socket socket, const std::string& data)
{
    std::size_t sent = 0;

    while (sent < data.size())
    {
        const int count =
            send(socket, data.data() + sent,
                 static_cast<int>(data.size() - sent), 0);

        if (count <= 0)
            return false;

        sent += static_cast<std::size_t>(count);
    }

    return true;
}

void sendResponse(Socket socket, int status, const std::string& body)
{
    const char* reason =
        status == 200 ? "OK" :
        status == 400 ? "Bad Request" :
        status == 404 ? "Not Found" :
                        "Unprocessable Entity";

    std::ostringstream response;

    response << "HTTP/1.1 " << status << ' ' << reason << "\r\n"
             << "Content-Type: application/json; charset=utf-8\r\n"
             << "Access-Control-Allow-Origin: *\r\n"
             << "Access-Control-Allow-Methods: POST, OPTIONS\r\n"
             << "Access-Control-Allow-Headers: Content-Type\r\n"
             << "Connection: close\r\n"
             << "Content-Length: " << body.size() << "\r\n\r\n"
             << body;

    sendAll(socket, response.str());
}

void handleClient(Socket client)
{
    std::string request;
    char buffer[4096];
    std::size_t headerEnd = std::string::npos;

    while ((headerEnd = request.find("\r\n\r\n")) == std::string::npos)
    {
        const int count = recv(client, buffer, sizeof(buffer), 0);

        if (count <= 0)
            return;

        request.append(buffer, static_cast<std::size_t>(count));

        if (request.size() > 1024 * 1024)
        {
            sendResponse(
                client,
                400,
                errorJson("Request headers are too large."));
            return;
        }
    }

    const std::string headers = request.substr(0, headerEnd);
    std::istringstream headerStream(headers);

    std::string method, path, version;
    headerStream >> method >> path >> version;

    if (method == "OPTIONS")
    {
        sendResponse(client, 200, "{}");
        return;
    }

    if (method != "POST" || path != "/api/solve")
    {
        sendResponse(client, 404, errorJson("Use POST /api/solve."));
        return;
    }

    std::size_t contentLength = 0;
    std::string line;

    std::getline(headerStream, line);

    while (std::getline(headerStream, line))
    {
        std::transform(
            line.begin(),
            line.end(),
            line.begin(),
            [](unsigned char ch)
            {
                return static_cast<char>(std::tolower(ch));
            });

        const auto colon = line.find(':');

        if (colon != std::string::npos &&
            line.substr(0, colon) == "content-length")
        {
            contentLength =
                static_cast<std::size_t>(std::stoul(line.substr(colon + 1)));
        }
    }

    const std::size_t bodyStart = headerEnd + 4;

    if (contentLength > 1024 * 1024)
    {
        sendResponse(
            client,
            400,
            errorJson("Request body is too large."));
        return;
    }

    while (request.size() - bodyStart < contentLength)
    {
        const int count = recv(client, buffer, sizeof(buffer), 0);

        if (count <= 0)
        {
            sendResponse(
                client,
                400,
                errorJson("Request body ended unexpectedly."));
            return;
        }

        request.append(buffer, static_cast<std::size_t>(count));
    }

    const auto [status, body] =
        solveRequest(request.substr(bodyStart, contentLength));

    sendResponse(client, status, body);
}
}

int runHttpApi(unsigned short port)
{
#ifdef _WIN32
    WSADATA startupData;

    if (WSAStartup(MAKEWORD(2, 2), &startupData) != 0)
        throw std::runtime_error(
            "Could not initialize Windows sockets.");
#endif

    const Socket server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server == INVALID_SOCKET_VALUE)
        throw std::runtime_error("Could not create the API socket.");

    int reuse = 1;

    setsockopt(
        server,
        SOL_SOCKET,
        SO_REUSEADDR,
        reinterpret_cast<const char*>(&reuse),
        sizeof(reuse));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(
            server,
            reinterpret_cast<sockaddr*>(&address),
            sizeof(address)) != 0 ||
        listen(server, 8) != 0)
    {
        closeSocket(server);

#ifdef _WIN32
        WSACleanup();
#endif

        throw std::runtime_error(
            "Could not listen on 127.0.0.1:" +
            std::to_string(port) +
            ". Is the port already in use?");
    }

    std::cout << "KCubixLab solver API listening at "
              << "http://127.0.0.1:" << port << "/api/solve\n";

    while (true)
    {
        const Socket client = accept(server, nullptr, nullptr);

        if (client == INVALID_SOCKET_VALUE)
            continue;

        try
        {
            handleClient(client);
        }
        catch (const std::exception& error)
        {
            sendResponse(client, 400, errorJson(error.what()));
        }

        closeSocket(client);
    }
}

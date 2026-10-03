# Cube Solver (CFOP)

KCubixLab is a 3D Rubik's Cube solver built around a C++ solving pipeline and a React-based visualizer. It uses an A* search for the White Cross and a dedicated integration layer, `LegacyStageSupport`, to connect the F2L, OLL, and PLL stage routines to the current cube engine, then presents the solution one stage at a time.

## Project overview

The project represents a standard 3×3 cube in code, generates or accepts move scrambles, and solves cube states through a local C++ HTTP API. The browser frontend renders the cube in 3D, supports manual face turns, and animates the returned solution by stage.

## Project image

![KCubixLab application preview](image.png)

*KCubixLab cube visualizer and solver interface.*

## Features

- Random 22-move scrambles and custom scrambles using standard face-turn notation
- A* search for the White Cross, followed by integrated F2L, OLL, and PLL stages
- Verification after every solving stage
- Adjacent same-face move simplification for F2L, OLL, and PLL output
- Interactive 3D cube with orbit controls and animated turns
- Stage-by-stage playback with play, pause, previous-move, and next-move controls
- Manual clockwise, inverse, and half turns for all six faces
- Light and dark themes, with the selected theme stored in browser local storage
- C++ pipeline tests and JavaScript cube-state tests

## Solving approach

The solver uses a hybrid architecture: the Cross stage is implemented with the project's dedicated A* solver, while F2L, OLL, and PLL are executed through `LegacyStageSupport`, a dedicated integration layer that connects the stage-solving logic to the project's `RubixCube` state and move system. This is not a Kociemba or two-phase solver, and the complete solution is not claimed to be globally optimal.

```text
Scrambled cube
      │
      ▼
White Cross ── A* search
      │
      ▼
F2L ──────────┐
      │       │
      ▼       │
OLL ──────────┼── LegacyStageSupport
      │       │
      ▼       │
PLL ──────────┘
      │
      ▼
Solved cube
```

### How CFOP works

The solver follows the standard CFOP stage order. The Cross is solved directly by `CrossSolver`; F2L, OLL, and PLL use `LegacyStageSupport` as the bridge between the current `RubixCube` representation and the internal representation used by their stage routines.

#### Cross — White Cross

`CrossSolver` uses A* search to find a solution for the four white cross edges. The resulting moves are applied to the project cube before the next stage begins.

#### F2L — First Two Layers

F2L solves the four corner-edge pairs that complete the first two layers.

The process is:

1. `F2LSolver` checks that the Cross is already solved.
2. `LegacyStageSupport` converts the current `RubixCube` state into the representation expected by the F2L routine.
3. The F2L routine tests different orders for solving the four slots and keeps the shortest complete sequence it finds.
4. The generated internal moves are captured, converted to the project's standard notation, and applied to the main `RubixCube`.
5. `F2LSolver` simplifies adjacent same-face moves and verifies that all four F2L slots are solved.

In short:

`RubixCube state → state conversion → F2L routine → move capture/conversion → RubixCube update → verification`

#### OLL — Orientation of the Last Layer

OLL orients the last-layer pieces so that the entire yellow face is oriented.

The process is:

1. `OLLSolver` checks that Cross and F2L are solved.
2. `LegacyStageSupport` rebuilds its internal representation from the current cube.
3. If necessary, it first creates the Yellow Cross.
4. It applies the OLL routine and checks the yellow face after each attempt, stopping when the face is fully oriented, with at most eight applications.
5. The moves are converted and applied to the main `RubixCube`, then simplified and verified by `OLLSolver`.

In short:

`RubixCube state → state conversion → Yellow Cross (if needed) → OLL routine → move synchronization → verification`

#### PLL — Permutation of the Last Layer

PLL positions the already-oriented last-layer pieces to finish the cube.

The process is split into two coordinated parts:

1. `PLLSolver` checks that OLL is solved.
2. `LegacyStageSupport` runs the corner-permutation routine and immediately applies those moves to the project cube.
3. Using that updated state, it runs the edge-permutation routine.
4. Both move sequences are combined, converted into project notation, simplified, and applied through the same synchronization layer.
5. `PLLSolver` verifies that every sticker matches the solved cube.

In short:

`RubixCube state → corner permutation → synchronize → edge permutation → combine moves → verification`

### LegacyStageSupport

`LegacyStageSupport` is not a separate solving stage. It is the integration layer that makes the F2L, OLL, and PLL routines work with the project's current cube engine.

For each stage it:

- **Prepares state:** rebuilds the internal cube representation from the current `RubixCube`.
- **Handles representation differences:** converts color IDs and face orientations where required.
- **Runs the stage routine:** invokes the appropriate F2L, OLL, or PLL logic.
- **Captures moves:** records the moves produced by that routine.
- **Converts notation:** translates the internal move encoding into project notation such as `R`, `R'`, and `U2`.
- **Synchronizes state:** executes those converted moves on the main `RubixCube`, keeping the project's cube state authoritative.
- **Resets stage state:** clears its internal data before each run so stages do not inherit stale state.

This keeps `F2LSolver`, `OLLSolver`, and `PLLSolver` as clean stage-level interfaces while isolating representation conversion and state synchronization inside one component.

## Solution pipeline

For an API request, the backend:

1. Parses and validates the supplied cube state.
2. Solves the White Cross with A* and applies those moves.
3. Passes the resulting cube state through `LegacyStageSupport`, which prepares the internal stage state and runs F2L, OLL, and PLL in sequence.
4. `LegacyStageSupport` converts each stage's internal state and move representation back into the project's cube model and move notation.
5. Verifies the cube after each stage and checks the final solved state.
6. Returns separate move arrays for the four stages; the frontend validates and displays them.

F2L, OLL, and PLL move sequences are simplified by combining adjacent turns of the same face. The simplifier preserves move order and does not change the solving algorithms. For example:

| Input | Output |
| --- | --- |
| `R R` | `R2` |
| `R R'` | removed |
| `R' R'` | `R2` |
| `R2 R` | `R'` |

## Frontend

The React interface renders 26 cubies with React Three Fiber and Drei orbit controls. Users can generate or enter a scramble, apply individual face turns, request a solution, and play the resulting move sequences stage by stage. Playback supports pausing, resuming, stepping backward or forward, and animating each turn in sequence. A theme toggle switches between light and dark appearance.

## Backend and API

The backend is written in C++17 and built with CMake. It contains cube-state and move handling, scramble generation, the A* Cross solver, the `LegacyStageSupport` integration layer for F2L, OLL, and PLL, move simplification, and correctness checks.

The backend exposes a lightweight HTTP API implemented directly with platform sockets; it does not use a web framework. The server binds to `127.0.0.1` and defaults to port `8080`.

### `POST /api/solve`

Request body:

```json
{
  "cube": [
    [[0, 0, 0], [0, 0, 0], [0, 0, 0]],
    [[1, 1, 1], [1, 1, 1], [1, 1, 1]],
    [[2, 2, 2], [2, 2, 2], [2, 2, 2]],
    [[3, 3, 3], [3, 3, 3], [3, 3, 3]],
    [[4, 4, 4], [4, 4, 4], [4, 4, 4]],
    [[5, 5, 5], [5, 5, 5], [5, 5, 5]]
  ]
}
```

The actual `cube` value contains six faces, each with three rows of three integer color IDs. Face order and center IDs are `WHITE=0`, `RED=1`, `BLUE=2`, `GREEN=3`, `ORANGE=4`, and `YELLOW=5`. The server checks the shape, color counts, and fixed center orientation before solving.

A successful response contains `cross`, `f2l`, `oll`, and `pll` arrays of move strings, plus a `counts` object. Invalid requests return a JSON `error` message with status `400`; solver failures return an error with status `422`. The API also handles `OPTIONS` requests for browser preflight.

The Vite development server proxies `/api` to `http://127.0.0.1:8080`. Set `VITE_API_URL` when the frontend needs a different API base URL.

## Technologies used

### Backend

- C++17 and the C++ standard library
- CMake
- Platform sockets: Winsock on Windows and POSIX sockets on other supported builds

### Frontend

- JavaScript ES modules and React 19
- Vite 8 for development and production builds
- Three.js rendering through React Three Fiber, with Drei for scene controls
- CSS for layout, responsive styling, and themes

### Development and validation

- Node.js and npm for frontend scripts and dependencies
- CTest with the `solver_pipeline_tests` C++ executable
- Node's built-in test runner for cube-state tests
- ESLint for frontend linting

## Requirements

- CMake 3.16 or newer
- A C++17-capable compiler
- Node.js and npm; the project does not declare a minimum Node.js or npm version
- A supported platform with the socket APIs used by the backend (Winsock on Windows or POSIX sockets elsewhere)
- No third-party C++ libraries are required

## Project structure

```text
KCubixLab/
├── backend/
│   ├── include/                 # Cube, solver, move, and API declarations
│   ├── src/                     # C++ implementation and server entry point
│   ├── tests/                   # C++ solver pipeline tests
│   └── tools/                   # Legacy algorithm data generation helper
├── frontend/
│   ├── public/                  # Browser icons
│   ├── src/
│   │   ├── Components/
│   │   │   ├── Cube/            # 3D cube and cube-state logic
│   │   │   └── UI_/             # Move input component
│   │   ├── assets/
│   │   ├── App.jsx
│   │   └── ...                  # App styles and entry point
│   ├── package.json
│   ├── package-lock.json
│   └── vite.config.js
├── image.png                    # Project preview
├── CMakeLists.txt
└── README.md
```

## Build and run

### Backend

From the repository root, configure and build the C++ targets:

```sh
cmake -S . -B build
cmake --build build --config Release
```

The executable runs a demo scramble and prints the solution stages when started without arguments:

```sh
./build/rubix_cube_solver
```

To start the API server, pass `--server`; an optional port can follow it, with `8080` as the default:

```sh
./build/rubix_cube_solver --server
./build/rubix_cube_solver --server 8081
```

On Windows with a multi-configuration CMake generator, the executable is commonly under `build/Release`:

```powershell
.\build\Release\rubix_cube_solver.exe --server
```

Single-configuration generators may place it directly in `build`. Keep the server running while using the frontend. The default Vite proxy expects it at `127.0.0.1:8080`.

To run the C++ pipeline tests after building:

```sh
ctest --test-dir build -C Release --output-on-failure
```

### Frontend

In a separate terminal:

```sh
cd frontend
npm install
npm run dev
```

Open the local URL printed by Vite. Other declared scripts are `npm run build`, `npm run preview`, `npm run lint`, and `npm run test:cube`.

## Usage

1. Start the backend API.
2. Start the frontend and open the URL printed by Vite.
3. Generate a random scramble or enter a custom sequence, then apply it to the cube.
4. Select **Solve cube** and wait for the backend to return the stages.
5. Inspect and play the White Cross, F2L, OLL, and PLL stages using the playback controls.
6. Use the manual move buttons to turn any face individually.

## Move notation

`R` means a clockwise turn of the right face, `R'` means a counter-clockwise turn, and `R2` means a 180° turn. The same suffixes apply to `U`, `D`, `L`, `F`, and `B` (up, down, left, front, and back).

## Validation and testing

The C++ API checks cube dimensions, color IDs, color counts, and fixed centers. It verifies the Cross, F2L, OLL, and PLL stages and the final solved state. The C++ test executable exercises solved and scrambled states and checks that returned stage moves reproduce the corresponding cube state. The frontend's Node tests cover move behavior, notation parsing, and cube serialization. Run them with:

```sh
cd frontend
npm run test:cube
```

## Design decisions

### Hybrid solver architecture

The Cross has a dedicated A* implementation, while `LegacyStageSupport` provides a clean integration boundary for F2L, OLL, and PLL. It isolates state conversion, move capture, notation conversion, and cube-state synchronization from the stage-level solver interfaces. This keeps the complete CFOP pipeline modular and verifiable without claiming a globally optimal solution.

### Stage separation

`CrossSolver`, `F2LSolver`, `OLLSolver`, and `PLLSolver` expose stage-specific solving and verification. Each later stage checks that its prerequisites are solved before it runs.

### Move simplification

Adjacent turns of the same face are combined or canceled to make stage playback more concise. Simplification happens after `LegacyStageSupport` has produced the stage sequence; it does not reorder moves or replace the underlying stage-solving logic.

## Limitations

- The solver targets a standard 3×3 cube with the fixed center orientation documented above.
- The Cross search uses a simple count of unsolved cross edges as its heuristic; search time and memory use depend on the input state.
- F2L, OLL, and PLL rely on the stage-solving routines exposed through `LegacyStageSupport` and its state/notation conversion layer.
- The full solution is not optimized globally for move count.

## License

No project-wide license file is included in this repository. The legacy algorithm source contains attribution for algorithm data adapted from the `cubing-algs` project; consult that source attribution and the upstream license when redistributing those portions.

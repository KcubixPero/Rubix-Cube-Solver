cmake_minimum_required(VERSION 3.16)

project(RubixCubeSolver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(rubixcube_core
    backend/src/RubixCube.cpp
    backend/src/MoveParser.cpp
    backend/src/Scrambler.cpp
    backend/src/CrossSolver.cpp
    backend/src/F2LSolver.cpp
    backend/src/AlgorithmNotation.cpp
)
target_include_directories(rubixcube_core PUBLIC backend/include)

add_executable(rubix_cube_solver backend/src/main.cpp)
target_link_libraries(rubix_cube_solver PRIVATE rubixcube_core)

include(CTest)
if(BUILD_TESTING)
    add_executable(f2l_solver_tests backend/tests/F2LSolverTests.cpp)
    target_link_libraries(f2l_solver_tests PRIVATE rubixcube_core)
    add_test(NAME f2l_solver_tests COMMAND f2l_solver_tests)

endif()

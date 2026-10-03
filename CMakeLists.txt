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
    backend/src/OLLSolver.cpp
    backend/src/PLLSolver.cpp
    backend/src/LegacyStageSupport.cpp
    backend/src/MoveSimplifier.cpp
    backend/src/HttpApi.cpp
)
target_include_directories(rubixcube_core PUBLIC backend/include)
if(WIN32)
    target_link_libraries(rubixcube_core PUBLIC ws2_32)
endif()

add_executable(rubix_cube_solver backend/src/main.cpp)
target_link_libraries(rubix_cube_solver PRIVATE rubixcube_core)

include(CTest)
if(BUILD_TESTING)
    add_executable(solver_pipeline_tests backend/tests/SolverPipelineTests.cpp)
    target_link_libraries(solver_pipeline_tests PRIVATE rubixcube_core)
    add_test(NAME solver_pipeline_tests COMMAND solver_pipeline_tests)

endif()

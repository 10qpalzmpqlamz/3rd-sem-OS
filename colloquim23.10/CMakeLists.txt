cmake_minimum_required(VERSION 3.15)
project(IndustrialSolver)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(solver_app 
    src/Solver.cpp
    src/main.cpp
)

target_include_directories(solver_app PUBLIC include)

if(GTest_FOUND)
    add_executable(solver_tests 
        tests/SolverTest.cpp
        src/Solver.cpp
    )

    target_include_directories(solver_tests PUBLIC include)
    target_link_libraries(solver_tests GTest::gtest GTest::gtest_main)

    enable_testing()
    add_test(NAME SolverTests COMMAND solver_tests)
else()
    message(WARNING "GTest not found - tests will not be built")
endif()
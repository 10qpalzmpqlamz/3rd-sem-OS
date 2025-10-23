cmake_minimum_required(VERSION 3.10)
project(lab1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options("/W4")
else()
    add_compile_options("-Wall" "-Wextra")
endif()

add_executable(lab1 main.cpp)

if(WIN32)
    target_link_libraries(lab1 Kernel32.lib)
endif()
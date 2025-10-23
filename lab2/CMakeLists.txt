cmake_minimum_required(VERSION 3.15)
project(lab2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options("/W4" "/permissive-")
else()
    add_compile_options("-Wall" "-Wextra" "-pedantic")
endif()

add_executable(lab2 main.cpp)

if(WIN32)
    target_link_libraries(lab2 Kernel32.lib)
endif()
cmake_minimum_required(VERSION 3.15)
project(MarkerThreads)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

if(WIN32)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

include_directories(include)

add_executable(marker_threads 
    src/main.cpp
    src/MarkerManager.cpp
)

add_executable(marker_tests 
    tests/MarkerManagerTest.cpp
    src/MarkerManager.cpp
)

if(MSVC)
    target_compile_options(marker_threads PRIVATE /W4)
    target_compile_options(marker_tests PRIVATE /W4)
else()
    target_compile_options(marker_threads PRIVATE -Wall -Wextra)
    target_compile_options(marker_tests PRIVATE -Wall -Wextra)
endif()

if(WIN32)
    target_compile_definitions(marker_threads PRIVATE _WIN32_WINNT=0x0600)
    target_compile_definitions(marker_tests PRIVATE _WIN32_WINNT=0x0600)
    
    target_link_libraries(marker_threads kernel32 user32)
    target_link_libraries(marker_tests 
        kernel32 
        user32 
        GTest::gtest 
        GTest::gtest_main
    )
else()
    target_link_libraries(marker_tests 
        GTest::gtest 
        GTest::gtest_main 
        pthread
    )
endif()

target_link_libraries(marker_tests 
    GTest::gtest 
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(marker_tests)

set_target_properties(marker_threads PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(marker_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.20)

# Define the project name and specify the C++ standard
project(bignum LANGUAGES CXX)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(dependencies/googletest)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -DDEBUG")
    message(STATUS "Building in Debug mode")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
    message(STATUS "Building in Release mode")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -DDEBUGOPTIMIZED")
    message(STATUS "Building in DebugOptimized mode")
else()
    message(WARNING "Unknown build type. Defaulting to Release mode.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
endif()

add_compile_options(-march=native -flto=auto)

add_library(hw4_lib STATIC
    lib/bignum.cpp
)

target_include_directories(hw4_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(bignum src/part1.cpp)
target_link_libraries(bignum PRIVATE hw4_lib)

# tests
# Add tests to CMake
enable_testing()
include(GoogleTest)

add_executable(test_uint64 tests/test_uint64_operations.cpp)
target_link_libraries(test_uint64 PRIVATE hw4_lib gtest_main)
gtest_discover_tests(test_uint64)

add_executable(test_init tests/test_init.cpp)
target_link_libraries(test_init PRIVATE hw4_lib gtest_main)
gtest_discover_tests(test_init)




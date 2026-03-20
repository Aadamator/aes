cmake_minimum_required(VERSION 3.21)
project(
    aes
    C
    CXX # C++ needed for benchmarking
)

##
# fetch project dependencies
##
include(FetchContent)

# unity - assertation library.
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG        v2.5.2
)
FetchContent_MakeAvailable(unity)

# libsodium - cryptographic library.
FetchContent_Declare(
    sodium
    GIT_REPOSITORY https://github.com/robinlinden/libsodium-cmake.git
    GIT_TAG cfebfd3da486d5a86c644c8b47067e5411c7599c # HEAD, last updated at 2026-02-28
)
set(SODIUM_DISABLE_TESTS ON)
FetchContent_MakeAvailable(sodium)

# benchmark - benchmarking library.
FetchContent_Declare(
    google_benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.9.5
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark tests" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Disable benchmark install" FORCE)
FetchContent_MakeAvailable(google_benchmark)

# mbedtls - cryptographic library.
FetchContent_Declare(
    mbedtls
    GIT_REPOSITORY https://github.com/Mbed-TLS/mbedtls.git
    GIT_TAG        v3.6.0
)
set(ENABLE_TESTING OFF CACHE BOOL "Disable mbedtls tests" FORCE)
set(ENABLE_PROGRAMS OFF CACHE BOOL "Disable mbedtls programs" FORCE)
FetchContent_MakeAvailable(mbedtls)

##
# configuration
##

enable_testing()

##
# add project directories
##

# source directories
add_subdirectory(lib)
add_subdirectory(include/common)
add_subdirectory(impls/naive)
add_subdirectory(impls/optimized)
add_subdirectory(impls/t_tables)
add_subdirectory(impls/aes_ni)

# testing directory
add_subdirectory(tests)

# benchmarking directory
add_subdirectory(benchmarks)

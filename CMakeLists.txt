cmake_minimum_required(VERSION 3.21)
project(aes C)

# configure testing
include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG        v2.5.2
)
FetchContent_MakeAvailable(unity)
FetchContent_Declare(
    Sodium
    GIT_REPOSITORY https://github.com/robinlinden/libsodium-cmake.git
    GIT_TAG cfebfd3da486d5a86c644c8b47067e5411c7599c # HEAD, last updated at 2026-02-28
)
set(SODIUM_DISABLE_TESTS ON)
FetchContent_MakeAvailable(Sodium)

enable_testing()

# add source directories
add_subdirectory(include/common)
add_subdirectory(impls/naive)
add_subdirectory(impls/optimized)
add_subdirectory(impls/t_tables)
add_subdirectory(impls/aes_ni)

# add testing directories
add_subdirectory(tests)

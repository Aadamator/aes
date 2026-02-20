cmake_minimum_required(VERSION 3.21)
project(aes C)

# fetch dependencies
include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG        v2.5.2
)
FetchContent_MakeAvailable(unity)

enable_testing()

# add source directories
add_subdirectory(include/common)
add_subdirectory(impls/aes_ni)
add_subdirectory(impls/naive)
add_subdirectory(impls/optimized)
add_subdirectory(impls/t_tables)

# add testing directories
add_subdirectory(tests)

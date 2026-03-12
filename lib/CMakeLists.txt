add_library(lib STATIC constants.c state_utils.c)

# export the headers
target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

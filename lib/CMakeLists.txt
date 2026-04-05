add_library(aes_lib
    STATIC
    constants.c
    gf.c
    keys.c
    s_box.c
    state_utils.c
)

# export the headers
target_include_directories(aes_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(aes_lib
    STATIC
    gf.c
    keys.c
    matrix.c
    s_box.c
)

# export the headers
target_include_directories(aes_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

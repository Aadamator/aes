add_library(aes_128_ni_instructions_impl STATIC aes_128.c)

# use include/common headers
target_link_libraries(aes_128_ni_instructions_impl PUBLIC aes_128_interface aes_lib)

# enable ni instructions for this target
target_compile_options(aes_128_ni_instructions_impl PRIVATE -maes)


# use alias
add_library(aes_128_ni_instructions ALIAS aes_128_ni_instructions_impl)

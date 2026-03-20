add_library(aes_128_optimized_impl STATIC aes_128.c)

# use include/common headers
target_link_libraries(aes_128_optimized_impl PUBLIC aes_128_interface aes_lib)

# use alias
add_library(aes_128_optimized ALIAS aes_128_optimized_impl)

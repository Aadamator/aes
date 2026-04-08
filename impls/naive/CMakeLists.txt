add_library(aes_128_naive_impl STATIC aes_128.c)

# link library headers
target_link_libraries(aes_128_naive_impl PUBLIC aes_lib)

# use alias
add_library(aes_128_naive ALIAS aes_128_naive_impl)

add_library(aes_128_ni_impl STATIC aes_128.c)

# use include/common headers
target_link_libraries(aes_128_ni_impl PUBLIC aes_128_interface lib)

# use alias
add_library(aes_128_ni ALIAS aes_128_ni_impl)

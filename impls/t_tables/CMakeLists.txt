add_library(aes_128_t_tables_impl STATIC aes_128.c)

# use include/common headers
target_link_libraries(aes_128_t_tables_impl PUBLIC aes_lib)

# use alias
add_library(aes_128_t_tables ALIAS aes_128_t_tables_impl)

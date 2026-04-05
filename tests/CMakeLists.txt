function(add_aes_test test_name aes_implementation_name test_source)
    file(GLOB
        TEST_UTILITY_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/utilities/*.c"
    )

    # create the C executable
    add_executable(${test_name} ${test_source} ${TEST_UTILITY_SOURCES})

    # link the specific implementation and libraries
    target_link_libraries(${test_name}
        PRIVATE
        aes_128_interface           # shared header
        ${aes_implementation_name}
        aes_lib
        sodium
        unity
    )

    # add test
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

# aes_ni tests
add_aes_test(aes_ni_decrypt_tests aes_128_ni decrypt.c)
add_aes_test(aes_ni_encrypt_tests aes_128_ni encrypt.c)

# naive tests
add_aes_test(naive_decrypt_tests aes_128_naive decrypt.c)
add_aes_test(naive_encrypt_tests aes_128_naive encrypt.c)

# optimized tests
add_aes_test(optimized_decrypt_tests aes_128_optimized decrypt.c)
add_aes_test(optimized_encrypt_tests aes_128_optimized encrypt.c)

# t_tables tests
add_aes_test(t_tables_decrypt_tests aes_128_t_tables decrypt.c)
add_aes_test(t_tables_encrypt_tests aes_128_t_tables encrypt.c)

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

# naive tests
add_aes_test(test_naive_decrypt aes_128_naive test_decrypt.c)
add_aes_test(test_naive_encrypt aes_128_naive test_encrypt.c)
add_aes_test(test_naive_encrypt_rounds aes_128_naive test_encrypt_rounds.c)

# ni_instructions tests
add_aes_test(test_ni_instructions_decrypt aes_128_ni_instructions test_decrypt.c)
add_aes_test(test_ni_instructions_encrypt aes_128_ni_instructions test_encrypt.c)

# optimized tests
add_aes_test(test_optimized_decrypt aes_128_optimized test_decrypt.c)
add_aes_test(test_optimized_encrypt aes_128_optimized test_encrypt.c)

# t_tables tests
add_aes_test(test_t_tables_decrypt aes_128_t_tables test_decrypt.c)
add_aes_test(test_t_tables_encrypt aes_128_t_tables test_encrypt.c)

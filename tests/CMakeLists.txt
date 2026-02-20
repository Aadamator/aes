# aes_ni tests
add_executable(aes_ni_tests unit_tests.c)
target_link_libraries(aes_ni_tests PRIVATE aes_128_interface aes_128_ni unity)
add_test(NAME aes_ni_tests COMMAND aes_ni_tests)

# naive tests
add_executable(naive_tests unit_tests.c)
target_link_libraries(naive_tests PRIVATE aes_128_interface aes_128_naive unity)
add_test(NAME naive_tests COMMAND naive_tests)

# optimized tests
add_executable(optimized_tests unit_tests.c)
target_link_libraries(optimized_tests PRIVATE aes_128_interface aes_128_optimized unity)
add_test(NAME optimized_tests COMMAND optimized_tests)

# t_tables tests
add_executable(t_tables_tests unit_tests.c)
target_link_libraries(t_tables_tests PRIVATE aes_128_interface aes_128_t_tables unity)
add_test(NAME t_tables_tests COMMAND t_tables_tests)

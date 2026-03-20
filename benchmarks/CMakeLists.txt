set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB
    BENCHMARK_UTILITY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/utilities/*.cpp"
)

function(add_aes_benchmark benchmark_name aes_implementation_name benchmark_source)
    # create the C++ executable
    add_executable(${benchmark_name} ${benchmark_source} ${BENCHMARK_UTILITY_SOURCES})

    # link the specific implementation and libraries
    target_link_libraries(${benchmark_name}
        PRIVATE
        aes_128_interface           # shared header
        ${aes_implementation_name}
        benchmark::benchmark
        benchmark::benchmark_main
    )

    # pass the library name to C++ so the JSON output knows which implementation this is
    target_compile_definitions(
        ${benchmark_name}
        PRIVATE
        AES_IMPLEMENTATION_NAME="${aes_implementation_name}"
    )
endfunction()

function(add_library_benchmark direction)
    # create the C++ executable
    add_executable(library_${direction}_benchmark
        library_${direction}.cpp
        ${BENCHMARK_UTILITY_SOURCES}
    )

    # link libraries
    target_link_libraries(library_${direction}_benchmark
        PRIVATE
        benchmark::benchmark
        benchmark::benchmark_main
        mbedcrypto                  # Mbed-TLS thord-party library
    )
endfunction()

# aes_ni benchmarks
add_aes_benchmark(aes_ni_encrypt_benchmark aes_128_ni encrypt.cpp)
#add_aes_benchmark(aes_ni_decrypt_benchmark aes_128_ni decrypt.cpp)

# naive benchmarks
add_aes_benchmark(naive_encrypt_benchmark aes_128_naive encrypt.cpp)
#add_aes_benchmark(naive_decrypt_benchmark aes_128_naive decrypt.cpp)

# optimized benchmarks
add_aes_benchmark(optimized_encrypt_benchmark aes_128_optimized encrypt.cpp)
#add_aes_benchmark(optimized_decrypt_benchmark aes_128_optimized decrypt.cpp)

# t_tables benchmarks
add_aes_benchmark(t_tables_encrypt_benchmark aes_128_t_tables encrypt.cpp)
#add_aes_benchmark(t_tables_decrypt_benchmark aes_128_t_tables decrypt.cpp)

# third party library benchmarks
add_library_benchmark(encrypt)
#add_openssl_benchmark(decrypt)

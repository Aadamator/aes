set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB
    BENCHMARK_UTILITY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/utilities/*.cpp"
)

function(add_aes_benchmark benchmark_name aes_implementation name benchmark_source)
    # create the c++ executable
    add_executable(${benchmark_name} ${benchmark_source} ${BENCHMARK_UTILITY_SOURCES})

    # link the specific implementation and libraries
    target_link_libraries(${benchmark_name}
        PRIVATE
        aes_128_interface           # shared header
        ${aes_implementation}
        aes_lib
        benchmark::benchmark
        benchmark::benchmark_main
    )

    # pass the implementation name to C++
    target_compile_definitions(${benchmark_name}
        PRIVATE
        AES_IMPLEMENTATION_NAME="${name}"
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

    # pass the library name to C++
    target_compile_definitions(library_${direction}_benchmark
        PRIVATE
        "AES_LIBRARY_NAME=\"Library (Mbed-TLS)\""
    )
endfunction()

# naive benchmarks
add_aes_benchmark(naive_encrypt_benchmark aes_128_naive "Naive" encrypt.cpp)
add_aes_benchmark(naive_decrypt_benchmark aes_128_naive "Naive" decrypt.cpp)

# ni_instructions benchmarks
add_aes_benchmark(ni_instructions_encrypt_benchmark aes_128_ni_instructions "NI Instructions"
        encrypt.cpp)
add_aes_benchmark(ni_instructions_decrypt_benchmark aes_128_ni_instructions "NI Instructions"
        decrypt.cpp)

# optimized benchmarks
add_aes_benchmark(optimized_encrypt_benchmark aes_128_optimized "Optimized" encrypt.cpp)
add_aes_benchmark(optimized_decrypt_benchmark aes_128_optimized "Optimized" decrypt.cpp)

# t_tables benchmarks
add_aes_benchmark(t_tables_encrypt_benchmark aes_128_t_tables "T-tables" encrypt.cpp)
add_aes_benchmark(t_tables_decrypt_benchmark aes_128_t_tables "T-tables" decrypt.cpp)

# third party library benchmarks
add_library_benchmark(encrypt)
add_library_benchmark(decrypt)

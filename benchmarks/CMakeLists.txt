set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB
    BENCHMARK_UTILITY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/utilities/*.cpp"
)

function(add_aes_benchmark benchmark_name
    implementation_name
    implementation_source_alias
    benchmark_source
)
    # create the c++ executable
    add_executable(${benchmark_name}
        ${benchmark_source}
        ${BENCHMARK_UTILITY_SOURCES}
    )

    target_include_directories(${benchmark_name}
        PRIVATE
        ${CMAKE_SOURCE_DIR}
    )

    # link the specific implementation and libraries
    target_link_libraries(${benchmark_name}
        PRIVATE
        ${implementation_source_alias}
        aes_lib
        benchmark::benchmark
        benchmark::benchmark_main
    )

    # pass the implementation name to C++
    target_compile_definitions(${benchmark_name}
        PRIVATE
        IMPLEMENTATION_NAME="${implementation_name}"
    )
endfunction()

function(add_library_benchmark
    direction
)
    # create the c++ executable
    add_executable(benchmark_library_${direction}
        impls/library/benchmark_${direction}.cpp
        ${BENCHMARK_UTILITY_SOURCES}
    )

    # link libraries
    target_link_libraries(benchmark_library_${direction}
        PRIVATE
        benchmark::benchmark
        benchmark::benchmark_main
        mbedcrypto                  # Mbed-TLS thord-party library
    )

    # pass the library name to C++
    target_compile_definitions(benchmark_library_${direction}
        PRIVATE
        "IMPLEMENTATION_NAME=\"Library (Mbed-TLS)\""
    )
endfunction()

# naive benchmarks
add_aes_benchmark(benchmark_naive_encrypt
    "Naive"
    aes_128_naive
    impls/naive/benchmark_encrypt.cpp
)
add_aes_benchmark(benchmark_naive_decrypt
    "Naive"
    aes_128_naive
    impls/naive/benchmark_decrypt.cpp
)

# ni_instructions benchmarks
add_aes_benchmark(benchmark_ni_instructions_encrypt
    "NI Instructions"
    aes_128_ni_instructions
    impls/ni_instructions/benchmark_encrypt.cpp
)
add_aes_benchmark(benchmark_ni_instructions_decrypt
    "NI Instructions"
    aes_128_ni_instructions
    impls/ni_instructions/benchmark_decrypt.cpp
)

# optimized benchmarks
add_aes_benchmark(benchmark_optimized_encrypt
    "Optimized"
    aes_128_optimized
    impls/optimized/benchmark_encrypt.cpp
)
add_aes_benchmark(benchmark_optimized_decrypt
    "Optimized"
    aes_128_optimized
    impls/optimized/benchmark_decrypt.cpp
)

# t_tables benchmarks
add_aes_benchmark(benchmark_t_tables_encrypt
    "T-tables"
    aes_128_t_tables
    impls/t_tables/benchmark_encrypt.cpp
)
add_aes_benchmark(benchmark_t_tables_decrypt
    "T-tables"
    aes_128_t_tables
    impls/t_tables/benchmark_decrypt.cpp
)

# third party library benchmarks
add_library_benchmark(encrypt)
add_library_benchmark(decrypt)

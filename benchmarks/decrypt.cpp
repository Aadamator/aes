#include <benchmark/benchmark.h>
#include <string.h>

#include "utilities/constants.h"
#include "utilities/hex.h"

extern "C" {
  #include "aes_128.h"
}

static void BMDecrypt(benchmark::State& state) {
  uint8_t ciphertext[16];
  uint8_t key[16];
  uint8_t plaintext[16] = {0};

  hex_to_bytes(key_as_hex, key);
  hex_to_bytes(ciphertext_as_hex, ciphertext);

  for (auto _ : state) {
    decrypt(key, ciphertext, plaintext);

    benchmark::DoNotOptimize(plaintext);
    benchmark::ClobberMemory();
  }
}

BENCHMARK(BMDecrypt)->Name(AES_IMPLEMENTATION_NAME);

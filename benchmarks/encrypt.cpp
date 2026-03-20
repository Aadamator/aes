#include <benchmark/benchmark.h>
#include <string.h>

#include "utilities/constants.h"
#include "utilities/hex.h"

extern "C" {
  #include "aes_128.h"
}

static void BMEncrypt(benchmark::State& state) {
  uint8_t ciphertext[16] = {0};
  uint8_t key[16];
  uint8_t plaintext[16];

  hex_to_bytes(key_as_hex, key);
  hex_to_bytes(plaintext_as_hex, plaintext);

  for (auto _ : state) {
    encrypt(key, plaintext, ciphertext);

    benchmark::DoNotOptimize(ciphertext);
    benchmark::ClobberMemory();
  }
}

BENCHMARK(BMEncrypt)->Name(AES_IMPLEMENTATION_NAME);

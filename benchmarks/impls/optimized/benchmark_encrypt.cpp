#include <benchmark/benchmark.h>
#include <string.h>

#include "../../utilities/constants.h"
#include "../../utilities/hex.h"

extern "C" {
  #include "impls/optimized/aes_128.h"
  #include "keys.h"
}

static void BMEncrypt(benchmark::State& state) {
  uint8_t ciphertext[16] = {0};
  uint8_t key[16];
  uint8_t key_schedules[11][16] = {0};
  uint8_t plaintext[16];

  hex_to_bytes(key_as_hex, key);
  hex_to_bytes(plaintext_as_hex, plaintext);

  // pre-compute the round keys
  key_expansion(key, key_schedules);

  for (auto _ : state) {
    encrypt(key_schedules, plaintext, ciphertext);

    benchmark::DoNotOptimize(ciphertext);
    benchmark::ClobberMemory();
  }
}

BENCHMARK(BMEncrypt)->Iterations(iterations)->Repetitions(repetitions)->Name(IMPLEMENTATION_NAME);

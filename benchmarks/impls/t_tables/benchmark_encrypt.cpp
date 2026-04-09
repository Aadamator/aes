#include <benchmark/benchmark.h>
#include <string.h>

#include "../../utilities/constants.h"
#include "../../utilities/hex.h"

extern "C" {
  #include "impls/t_tables/aes_128.h"
}

static void BMEncrypt(benchmark::State& state) {
  uint8_t ciphertext[16] = {0};
  uint8_t key[16];
  uint32_t key_schedules[44] = {0};
  uint8_t plaintext[16];
  uint32_t t_tables[4][256] = {0};

  hex_to_bytes(key_as_hex, key);
  hex_to_bytes(plaintext_as_hex, plaintext);

  // pre-compute the round keys and t-tables
  key_schedule(key, key_schedules);
  generate_tables(t_tables);

  for (auto _ : state) {
    encrypt(key_schedules, t_tables, plaintext, ciphertext);

    benchmark::DoNotOptimize(ciphertext);
    benchmark::ClobberMemory();
  }
}

BENCHMARK(BMEncrypt)->Iterations(iterations)->Repetitions(repetitions)->Name(IMPLEMENTATION_NAME);

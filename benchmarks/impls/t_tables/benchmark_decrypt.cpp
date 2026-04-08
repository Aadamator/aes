#include <benchmark/benchmark.h>
#include <string.h>

#include "../../utilities/constants.h"
#include "../../utilities/hex.h"

extern "C" {
  #include "impls/t_tables/aes_128.h"
}

static void BMDecrypt(benchmark::State& state) {
  uint8_t ciphertext[16];
  uint8_t key[16];
  uint32_t key_schedules[44] = {0};
  uint8_t plaintext[16] = {0};
  uint32_t t_tables[4][256] = {0};

  hex_to_bytes(key_as_hex, key);
  hex_to_bytes(ciphertext_as_hex, ciphertext);

  // pre-compute the round keys and t-tables
  key_schedule(key, key_schedules);
  generate_tables(t_tables);

  for (auto _ : state) {
    decrypt(key_schedules, t_tables, ciphertext, plaintext);

    benchmark::DoNotOptimize(plaintext);
    benchmark::ClobberMemory();
  }
}

BENCHMARK(BMDecrypt)->Iterations(iterations)->Repetitions(repetitions)->Name(IMPLEMENTATION_NAME);

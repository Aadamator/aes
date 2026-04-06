#include <benchmark/benchmark.h>
#include <mbedtls/aes.h>
#include <string.h>

#include "utilities/constants.h"
#include "utilities/hex.h"

static void BMLibraryDecrypt(benchmark::State& state) {
  mbedtls_aes_context aes;
  uint8_t ciphertext[16];
  uint8_t key[16];
  uint8_t plaintext[16] = {0};

  hex_to_bytes(key_as_hex, key);
  hex_to_bytes(ciphertext_as_hex, ciphertext);

  // initialize decryption context
  mbedtls_aes_init(&aes);

  // add key to decryption context
  mbedtls_aes_setkey_dec(&aes, key, 128);

  for (auto _ : state) {
    mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, ciphertext, plaintext);

    benchmark::DoNotOptimize(plaintext);
    benchmark::ClobberMemory();
  }

  // release context
  mbedtls_aes_free(&aes);
}

BENCHMARK(BMLibraryDecrypt)->Iterations(iterations)->Repetitions(repetitions)->Name(AES_LIBRARY_NAME);

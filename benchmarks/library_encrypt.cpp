#include <benchmark/benchmark.h>
#include <mbedtls/aes.h>
#include <string.h>

#include "utilities/constants.h"
#include "utilities/hex.h"

static void BMLibraryEncrypt(benchmark::State& state) {
  mbedtls_aes_context aes;
  uint8_t ciphertext[16] = {0};
  uint8_t key[16];
  uint8_t plaintext[16];

  hex_to_bytes(key_as_hex, key);
  hex_to_bytes(plaintext_as_hex, plaintext);

  // initialize encryption context
  mbedtls_aes_init(&aes);

  // add key to encryption context
  mbedtls_aes_setkey_enc(&aes, key, 128);

  for (auto _ : state) {
    mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, plaintext, ciphertext);

    benchmark::DoNotOptimize(ciphertext);
    benchmark::ClobberMemory();
  }

  // release context
  mbedtls_aes_free(&aes);
}

BENCHMARK(BMLibraryEncrypt)->Name(AES_LIBRARY_NAME);

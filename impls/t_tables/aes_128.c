#include <stdint.h>
#include "aes_128.h"

/**
 * @brief Encrypts a single 16-byte block using AES-128 using T-tables.
 *
 * @param key           16-byte AES key.
 * @param plaintext     16-byte input block.
 * @param ciphertext    16-byte output block.
 * @return int          0 on success, non-zero on failure.
 */
int aes_128_encrypt(const uint8_t key[16], const uint8_t plaintext[16], uint8_t ciphertext[16]) {
    // placeholder, juse returns the plaintext
    for (int i = 0; i < 16; ++i) ciphertext[i] = plaintext[i];

    return 0;
}

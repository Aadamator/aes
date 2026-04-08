#pragma once

#include <stdint.h>

/**
 * @brief Decrypts a single 16-byte block using pre-computed T-tables, applying the inverse
 * operations of
 * the
 * encryption routine in the opposite order.
 *
 * @param key_schedules List of 16-byte pre-computed key schedules.
 * @param ciphertext    16-byte input block - the encrypted plaintext.
 * @param plaintext     16-byte output block - the plain text after decryption.
 * @return int          0 on success, non-zero on failure.
 */
int decrypt(
    const uint8_t key_schedules[11][16],
    const uint8_t ciphertext[16],
    uint8_t plaintext[16]
);

/**
 * @brief Encrypts a single 16-byte block using pre-computed T-tables.
 *
 * @param key_schedules List of 16-byte pre-computed key schedules.
 * @param plaintext     16-byte input block - the plaintext to encrypt.
 * @param ciphertext    16-byte output block - the encrypted plaintext.
 * @return int          0 on success, non-zero on failure.
 */
int encrypt(
    const uint8_t key_schedules[11][16],
    const uint8_t plaintext[16],
    uint8_t ciphertext[16]
);

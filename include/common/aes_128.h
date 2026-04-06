#pragma once

#include <stdint.h>

/**
 * @brief XOR the incoming ciphertext with the round key.
 *
 * @param round_key     16-byte round key.
 * @param state         16-byte current ciphertext.
 */
void add_round_key(const uint8_t round_key[16], uint8_t (*state)[16]);

/**
 * @brief Decrypts a single 16-byte block using AES-128, applying the inverse operations of the
 * encryption routine in the opposite order.
 *
 * @param key_schedules List of 16-byte pre-computed key schedules.
 * @param ciphertext    16-byte input block - the encrypted plaintext.
 * @param plaintext     16-byte output block - the plain text after decryption.
 * @return int          0 on success, non-zero on failure.
 */
int decrypt(
    const uint8_t key_schedules[11][16],
    const uint8_t (*ciphertext)[16],
    uint8_t (*plaintext)[16]
);

/**
 * @brief Encrypts a single 16-byte block using AES-128.
 *
 * @param key_schedules List of 16-byte pre-computed key schedules.
 * @param plaintext     16-byte input block - the plaintext to encrypt.
 * @param ciphertext    16-byte output block - the encrypted plaintext.
 * @return int          0 on success, non-zero on failure.
 */
int encrypt(
    const uint8_t key_schedules[11][16],
    const uint8_t (*plaintext)[16],
    uint8_t (*ciphertext)[16]
);

/**
 * @brief Inverse mix columns operation.
 *
 * @param state         16-byte current ciphertext.
 */
void inverse_mix_columns(uint8_t (*state)[16]);

/**
 * @brief Inverse shift rows operation.
 *
 * @param state         16-byte current ciphertext.
 */
void inverse_shift_rows(uint8_t (*state)[16]);

/**
 * @brief Mix the four bytes of every column in a linear way.
 *
 * @param state         16-byte current ciphertext.
 */
void mix_columns(uint8_t (*state)[16]);

/**
 * @brief Row 0 remains unchanged, but the other three rows are shifted a variable amount.
 *
 * @param state         16-byte current ciphertext.
 */
void shift_rows(uint8_t (*state)[16]);

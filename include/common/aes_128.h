#pragma once

#include <stdint.h>

/**
 * @brief XOR the incoming ciphertext with the round key.
 *
 * @param round_key     16-byte round key.
 * @param state         16-byte current ciphertext.
 */
void add_round_key(const uint8_t round_key[16], uint8_t state[16]);

/**
 * @brief Encrypts a single 16-byte block using AES-128.
 *
 * @param key           16-byte AES key.
 * @param plaintext     16-byte input block.
 * @param ciphertext    16-byte output block.
 * @return int          0 on success, non-zero on failure.
 */
int encrypt(
    const uint8_t key[16],
    const uint8_t plaintext[16],
    uint8_t ciphertext[16]
);

/**
 * @brief Precompute the key schedules from the cipher key.
 *
 * @param cipherkey     16-byte cipher key.
 * @param key_schedules 16-byte key schedules.
 */
void key_expansion(const uint8_t cipherkey[16], uint8_t key_schedules[11][16]);

/**
 * @brief Mix the four bytes of every column in a linear way.
 *
 * @param state         16-byte current ciphertext.
 */
void mix_columns(uint8_t state[16]);

/**
 * @brief Row 0 remains unchanged, but the other three rows are shifted a variable amount.
 *
 * @param direction     For encryption (0) or decryption (1+).
 * @param state         16-byte current ciphertext.
 */
void shift_rows(int direction, uint8_t state[16]);

/**
 * @brief Substitute each byte of the state with another byte according to a lookup table.
 *
 * @param state         16-byte current ciphertext.
 */
void sub_bytes(uint8_t state[16]);

#pragma once

#include <stdint.h>

/**
 * @brief Decrypts a single 16-byte block using pre-computed T-tables, applying the inverse
 * operations of
 * the
 * encryption routine in the opposite order.
 *
 * @param key_schedules A 44-byte pre-computed key schedule.
 * @param t_tables      List of 256-byte pre-computed T-tables.
 * @param ciphertext    16-byte input block - the encrypted plaintext.
 * @param plaintext     16-byte output block - the plain text after decryption.
 * @return int          0 on success, non-zero on failure.
 */
int decrypt(
    const uint32_t key_schedules[44],
    const uint32_t t_tables[4][256],
    const uint8_t ciphertext[16],
    uint8_t plaintext[16]
);

/**
 * @brief Encrypts a single 16-byte block using pre-computed T-tables.
 *
 * @param key_schedules A 44-byte pre-computed key schedule.
 * @param t_tables      List of 256-byte pre-computed T-tables.
 * @param plaintext     16-byte input block - the plaintext to encrypt.
 * @param ciphertext    16-byte output block - the encrypted plaintext.
 * @return int          0 on success, non-zero on failure.
 */
int encrypt(
    const uint32_t key_schedules[44],
    const uint32_t t_tables[4][256],
    const uint8_t plaintext[16],
    uint8_t ciphertext[16]
);

void generate_inverse_tables(uint32_t tables[4][256]);

void generate_tables(uint32_t tables[4][256]);

void key_schedule(const uint8_t *key, uint32_t *roundkey);

void reverse_round_keys(uint32_t *rk, uint32_t *inv_rk);

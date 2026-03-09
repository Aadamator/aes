#pragma once

#include <stdint.h>

void add_round_key(uint8_t state[16], const uint8_t round_key[16]);
int aes_128_encrypt(
    const uint8_t key[16],
    const uint8_t plaintext[16],
    uint8_t ciphertext[16]
);
void key_expansion(const uint8_t cipherkey[16], uint8_t key_schedules[11][16]);
void mix_columns(uint8_t state[16]);
void shift_rows(uint8_t state[16]);
void sub_bytes(uint8_t state[16]);

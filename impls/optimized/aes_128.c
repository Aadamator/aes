#include <stdint.h>

#include "aes_128.h"

void add_round_key(const uint8_t round_key[16], uint8_t state[4][4]) {}

int decrypt(const uint8_t key_schedules[11][16], const uint8_t ciphertext[16], uint8_t plaintext[16]) {
    // placeholder, just returns the cipertext
    for (int i = 0; i < 16; ++i) plaintext[i] = ciphertext[i];

    return 0;
}

int encrypt(const uint8_t key_schedules[11][16], const uint8_t plaintext[16], uint8_t ciphertext[16]) {
    // placeholder, just returns the plaintext
    for (int i = 0; i < 16; ++i) ciphertext[i] = plaintext[i];

    return 0;
}

void inverse_mix_columns(uint8_t state[4][4]) {}

void inverse_shift_rows(uint8_t state[4][4]) {}

void mix_columns(uint8_t state[4][4]) {}

void shift_rows(uint8_t state[4][4]) {}

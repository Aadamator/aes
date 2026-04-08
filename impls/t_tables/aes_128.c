#include <stdint.h>

#include "aes_128.h"

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

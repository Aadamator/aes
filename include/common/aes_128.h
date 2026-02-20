#pragma once

#include <stdint.h>

void aes_128_decrypt(
    const uint8_t key[16],
    const uint8_t ciphertext[16],
    uint8_t plaintext[16]
);

void aes_128_encrypt(
    const uint8_t key[16],
    const uint8_t plaintext[16],
    uint8_t ciphertext[16]
);

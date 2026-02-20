#pragma once

#include <stdint.h>

int aes_128_decrypt(
    const uint8_t key[16],
    const uint8_t ciphertext[16],
    uint8_t plaintext[16]
);

int aes_128_encrypt(
    const uint8_t key[16],
    const uint8_t plaintext[16],
    uint8_t ciphertext[16]
);

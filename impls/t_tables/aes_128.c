#include <stdint.h>
#include "aes_128.h"

void aes_128_encrypt(const uint8_t key[16], const uint8_t plaintext[16], uint8_t ciphertext[16]) {
    // placeholder, juse returns the plaintext
    for (int i = 0; i < 16; ++i) ciphertext[i] = plaintext[i];
}

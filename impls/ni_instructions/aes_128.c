#include <stdint.h>
#include <string.h>
#include <wmmintrin.h>

#include "aes_128.h"

#include "constants.h"
#include "s_box.h"

void add_round_key(const uint8_t round_key[16], uint8_t state[16]) {}

int decrypt(const uint8_t key[16], const uint8_t ciphertext[16], uint8_t plaintext[16]) {
    uint8_t key_schedules[11][16];
    __m128i round_key;
    __m128i state;

    // load the ciphertext into a 128-bit hardware register
    state = _mm_loadu_si128((const __m128i*)ciphertext);

    // create key schedules
    key_expansion(key, key_schedules);

    // initial key addition (starts with the last key schedule)
    round_key = _mm_loadu_si128((const __m128i*)key_schedules[10]);
    state = _mm_xor_si128(state, round_key);

    // rounds 9-1 are ordinary rounds
    for (int round = 9; round > 0; --round) {
        round_key = _mm_loadu_si128((const __m128i*)key_schedules[round]);

        // use inverse mix columns for decryption
        round_key = _mm_aesimc_si128(round_key);

        // perform inverse sub bytes and inverse shift rows usingt the hardware register
        state = _mm_aesdec_si128(state, round_key);
    }

    // final round ends with the extra key addition and the last round (no iverse mix columns) using the hardware register
    round_key = _mm_loadu_si128((const __m128i*)key_schedules[0]);
    state = _mm_aesdeclast_si128(state, round_key);

    // copy the state from the hardware register into the plaintext
    _mm_storeu_si128((__m128i*)plaintext, state);

    return 0;
}

int encrypt(const uint8_t key[16], const uint8_t plaintext[16], uint8_t ciphertext[16]) {
    uint8_t key_schedules[11][16];
    __m128i round_key;
    __m128i state;

    // load the plaintext into a 128-bit hardware register
    state = _mm_loadu_si128((const __m128i*)plaintext);

    // create key schedules
    key_expansion(key, key_schedules);

    // begin with a key addition (round 0) by xor-ing (add_round_key) directly using the hardware registers
    round_key = _mm_loadu_si128((const __m128i*)key_schedules[0]);
    state = _mm_xor_si128(state, round_key);

    // rounds 1-9 are ordinary rounds
    for (int round = 1; round < 10; round++) {
        round_key = _mm_loadu_si128((const __m128i*)key_schedules[round]);
        state = _mm_aesenc_si128(state, round_key);
    }

    // last round is special: as there are no mix_columns, a separate function is used for the final round
    round_key = _mm_loadu_si128((const __m128i*)key_schedules[10]);
    state = _mm_aesenclast_si128(state, round_key);

    // copy the state from the hardware register into the ciphertext
    _mm_storeu_si128((__m128i*)ciphertext, state);

    return 0;
}

void inverse_mix_columns(uint8_t state[16]) {}

void inverse_shift_rows(uint8_t state[16]) {}

void key_expansion(const uint8_t cipherkey[16], uint8_t key_schedules[11][16]) {
    // copy cipher key to the first position
    memcpy(key_schedules[0], cipherkey, 16);

    for (int i = 1; i < 11; i++) {
        uint8_t temp[4];
        memcpy(temp, key_schedules[i-1] + 12, 4);  // last column

        // rot-word
        uint8_t t = temp[0];
        temp[0] = temp[1]; temp[1] = temp[2]; temp[2] = temp[3]; temp[3] = t;

        // sub-word
        temp[0] = s_box[temp[0]];
        temp[1] = s_box[temp[1]];
        temp[2] = s_box[temp[2]];
        temp[3] = s_box[temp[3]];

        // xor r-con
        temp[0] ^= round_constants[i-1];

        // key_schedules[i] = key_schedules[i-1] xor (transformed word || 0)
        for (int j = 0; j < 4; j++) {
            key_schedules[i][j] = key_schedules[i-1][j] ^ temp[j];
        }

        for (int j = 4; j < 16; j++) {
            key_schedules[i][j] = key_schedules[i-1][j] ^ key_schedules[i][j-4];
        }
    }
}

void mix_columns(uint8_t state[16]) {}

void shift_rows(uint8_t state[16]) {}

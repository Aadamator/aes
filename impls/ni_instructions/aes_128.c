#include <stdint.h>
#include <wmmintrin.h>

#include "aes_128.h"

void add_round_key(const uint8_t round_key[16], uint8_t (*state)[16]) {}

int decrypt(const uint8_t key_schedules[11][16], const uint8_t (*ciphertext)[16], uint8_t (*plaintext)[16]) {
    __m128i round_key = _mm_loadu_si128((const __m128i*)key_schedules[10]); // initial key addition (starts with the last key schedule)
    __m128i state = _mm_loadu_si128((const __m128i*)ciphertext); // load the ciphertext into a 128-bit hardware register

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

int encrypt(const uint8_t key_schedules[11][16], const uint8_t (*plaintext)[16], uint8_t (*ciphertext)[16]) {
    __m128i round_key = _mm_loadu_si128((const __m128i*)key_schedules[0]); // begin with a key addition (round 0) by xor-ing (add_round_key) directly using the hardware registers
    __m128i state = _mm_loadu_si128((const __m128i*)plaintext); // load the plaintext into a 128-bit hardware register

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

void inverse_mix_columns(uint8_t (*state)[16]) {}

void inverse_shift_rows(uint8_t (*state)[16]) {}

void mix_columns(uint8_t (*state)[16]) {}

void shift_rows(uint8_t (*state)[16]) {}

#include <stdint.h>
#include <string.h>

#include "aes_128.h"

#include "s_box.h"

void add_round_key(const uint8_t round_key[16], uint8_t state[16]) {
    state[0]  ^= round_key[0];
    state[1]  ^= round_key[1];
    state[2]  ^= round_key[2];
    state[3]  ^= round_key[3];
    state[4]  ^= round_key[4];
    state[5]  ^= round_key[5];
    state[6]  ^= round_key[6];
    state[7]  ^= round_key[7];
    state[8]  ^= round_key[8];
    state[9]  ^= round_key[9];
    state[10] ^= round_key[10];
    state[11] ^= round_key[11];
    state[12] ^= round_key[12];
    state[13] ^= round_key[13];
    state[14] ^= round_key[14];
    state[15] ^= round_key[15];
}

int decrypt(const uint8_t key_schedules[11][16], const uint8_t ciphertext[16], uint8_t plaintext[16]) {
    uint8_t state[16] = {0};

    memcpy(state, ciphertext, 16);

    add_round_key(key_schedules[10], state);
    optimized_inverse_sub_bytes(state);
    inverse_shift_rows(state);

    for (int round = 9; round > 0; --round) {
        add_round_key(key_schedules[round], state);
        inverse_mix_columns(state);
        optimized_inverse_sub_bytes(state);
        inverse_shift_rows(state);
    }
    add_round_key(key_schedules[0], state);

    memcpy(plaintext, state, 16);

    return 0;
}

int encrypt(const uint8_t key_schedules[11][16], const uint8_t plaintext[16], uint8_t ciphertext[16]) {
    uint8_t state[16] = {0};

    memcpy(state, plaintext, 16);

    // begin with a key addition
    add_round_key(key_schedules[0], state);

    // rounds 1-9 are ordinary rounds
    for (int round = 1; round < 10; round++) {
        optimized_sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(key_schedules[round], state);
    }

    // last round is special: there is no mix_columns
    optimized_sub_bytes(state);
    shift_rows(state);
    add_round_key(key_schedules[10], state);

    memcpy(ciphertext, state, 16);

    return 0;
}

// xtime function: Perform multiplipilcation in GF(2^8)
static inline uint8_t xtime(uint8_t x) {
    return (x << 1) ^ (((x >> 7) & 1) * 0x1b);
}

// Helpers for InvMixColumns
static inline uint8_t mul9(uint8_t x)  { return xtime(xtime(xtime(x))) ^ x; }
static inline uint8_t mul11(uint8_t x) { return xtime(xtime(xtime(x))) ^ xtime(x) ^ x; }
static inline uint8_t mul13(uint8_t x) { return xtime(xtime(xtime(x))) ^ xtime(xtime(x)) ^ x; }
static inline uint8_t mul14(uint8_t x) { return xtime(xtime(xtime(x))) ^ xtime(xtime(x)) ^ xtime(x); }

void inverse_mix_columns(uint8_t state[16]) {
    uint8_t a, b, c, d;

    // Column 0
    a = state[0]; b = state[1]; c = state[2]; d = state[3];
    state[0] = mul14(a) ^ mul11(b) ^ mul13(c) ^ mul9(d);
    state[1] = mul9(a)  ^ mul14(b) ^ mul11(c) ^ mul13(d);
    state[2] = mul13(a) ^ mul9(b)  ^ mul14(c) ^ mul11(d);
    state[3] = mul11(a) ^ mul13(b) ^ mul9(c)  ^ mul14(d);

    // Column 1
    a = state[4]; b = state[5]; c = state[6]; d = state[7];
    state[4] = mul14(a) ^ mul11(b) ^ mul13(c) ^ mul9(d);
    state[5] = mul9(a)  ^ mul14(b) ^ mul11(c) ^ mul13(d);
    state[6] = mul13(a) ^ mul9(b)  ^ mul14(c) ^ mul11(d);
    state[7] = mul11(a) ^ mul13(b) ^ mul9(c)  ^ mul14(d);

    // Column 2
    a = state[8]; b = state[9]; c = state[10]; d = state[11];
    state[8] = mul14(a) ^ mul11(b) ^ mul13(c) ^ mul9(d);
    state[9] = mul9(a)  ^ mul14(b) ^ mul11(c) ^ mul13(d);
    state[10] = mul13(a) ^ mul9(b)  ^ mul14(c) ^ mul11(d);
    state[11] = mul11(a) ^ mul13(b) ^ mul9(c)  ^ mul14(d);

    // Column 3
    a = state[12]; b = state[13]; c = state[14]; d = state[15];
    state[12] = mul14(a) ^ mul11(b) ^ mul13(c) ^ mul9(d);
    state[13] = mul9(a)  ^ mul14(b) ^ mul11(c) ^ mul13(d);
    state[14] = mul13(a) ^ mul9(b)  ^ mul14(c) ^ mul11(d);
    state[15] = mul11(a) ^ mul13(b) ^ mul9(c)  ^ mul14(d);
}

void inverse_shift_rows(uint8_t state[16]) {
    uint8_t tmp;
    tmp = state[13]; state[13] = state[9]; state[9] = state[5]; state[5] = state[1]; state[1] = tmp;
    tmp = state[2]; state[2] = state[10]; state[10] = tmp;
    tmp = state[6]; state[6] = state[14]; state[14] = tmp;
    tmp = state[3]; state[3] = state[7]; state[7] = state[11]; state[11] = state[15]; state[15] = tmp;

}

void mix_columns(uint8_t state[16]) {
    uint8_t a, b, c, d;

    // Column 0 (Indices 0, 1, 2, 3)
    a = state[0]; b = state[1]; c = state[2]; d = state[3];
    state[0] = xtime(a) ^ (xtime(b) ^ b) ^ c ^ d;
    state[1] = a ^ xtime(b) ^ (xtime(c) ^ c) ^ d;
    state[2] = a ^ b ^ xtime(c) ^ (xtime(d) ^ d);
    state[3] = (xtime(a) ^ a) ^ b ^ c ^ xtime(d);

    // Column 1 (Indices 4, 5, 6, 7)
    a = state[4]; b = state[5]; c = state[6]; d = state[7];
    state[4] = xtime(a) ^ (xtime(b) ^ b) ^ c ^ d;
    state[5] = a ^ xtime(b) ^ (xtime(c) ^ c) ^ d;
    state[6] = a ^ b ^ xtime(c) ^ (xtime(d) ^ d);
    state[7] = (xtime(a) ^ a) ^ b ^ c ^ xtime(d);

    // Column 2 (Indices 8, 9, 10, 11)
    a = state[8]; b = state[9]; c = state[10]; d = state[11];
    state[8] = xtime(a) ^ (xtime(b) ^ b) ^ c ^ d;
    state[9] = a ^ xtime(b) ^ (xtime(c) ^ c) ^ d;
    state[10] = a ^ b ^ xtime(c) ^ (xtime(d) ^ d);
    state[11] = (xtime(a) ^ a) ^ b ^ c ^ xtime(d);

    // Column 3 (Indices 12, 13, 14, 15)
    a = state[12]; b = state[13]; c = state[14]; d = state[15];
    state[12] = xtime(a) ^ (xtime(b) ^ b) ^ c ^ d;
    state[13] = a ^ xtime(b) ^ (xtime(c) ^ c) ^ d;
    state[14] = a ^ b ^ xtime(c) ^ (xtime(d) ^ d);
    state[15] = (xtime(a) ^ a) ^ b ^ c ^ xtime(d);
}

void optimized_inverse_sub_bytes(uint8_t state[16]) {
    state[0] = inverse_s_box[state[0]];   state[1] = inverse_s_box[state[1]];   state[2] = inverse_s_box[state[2]];   state[3] = inverse_s_box[state[3]];
    state[4] = inverse_s_box[state[4]];   state[5] = inverse_s_box[state[5]];   state[6] = inverse_s_box[state[6]];   state[7] = inverse_s_box[state[7]];
    state[8] = inverse_s_box[state[8]];   state[9] = inverse_s_box[state[9]];   state[10] = inverse_s_box[state[10]]; state[11] = inverse_s_box[state[11]];
    state[12] = inverse_s_box[state[12]]; state[13] = inverse_s_box[state[13]]; state[14] = inverse_s_box[state[14]]; state[15] = inverse_s_box[state[15]];
}

void optimized_sub_bytes(uint8_t state[16]) {
    state[0] = s_box[state[0]];   state[1] = s_box[state[1]];   state[2] = s_box[state[2]];   state[3] = s_box[state[3]];
    state[4] = s_box[state[4]];   state[5] = s_box[state[5]];   state[6] = s_box[state[6]];   state[7] = s_box[state[7]];
    state[8] = s_box[state[8]];   state[9] = s_box[state[9]];   state[10] = s_box[state[10]]; state[11] = s_box[state[11]];
    state[12] = s_box[state[12]]; state[13] = s_box[state[13]]; state[14] = s_box[state[14]]; state[15] = s_box[state[15]];
}

void shift_rows(uint8_t state[16]) {
    uint8_t tmp;
    tmp = state[1]; state[1] = state[5]; state[5] = state[9]; state[9] = state[13]; state[13] = tmp;
    tmp = state[2]; state[2] = state[10]; state[10] = tmp;
    tmp = state[6]; state[6] = state[14]; state[14] = tmp;
    tmp = state[3]; state[3] = state[15]; state[15] = state[11]; state[11] = state[7]; state[7] = tmp;
}

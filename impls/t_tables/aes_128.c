#include <stdint.h>

#include "aes_128.h"

#include "s_box.h"

static const uint8_t RCON[10] ={
    0x01,0x02,0x04,0x08,
    0x10,0x20,0x40,0x80,
    0x1B,0x36
};

static inline uint8_t xtime(uint8_t x) {
    return (x << 1) ^ ((x & 0x80) ? 0x1B : 0x00);
}

static inline uint8_t mul3(uint8_t x) {
    return xtime(x) ^ x;
}

static inline uint8_t multi9(uint8_t x) {
    return xtime(xtime(xtime(x))) ^ x;
}


static inline uint8_t multi11(uint8_t x) {
    return xtime(xtime(xtime(x))) ^ xtime(x) ^ x;
}


static inline uint8_t multi13(uint8_t x) {
    return xtime(xtime(xtime(x))) ^ xtime(xtime(x)) ^ x;
}


static inline uint8_t multi14(uint8_t x) {
    return xtime(xtime(xtime(x))) ^ xtime(xtime(x)) ^ xtime(x);
}

static uint32_t rotword(uint32_t w) {
    return (w << 8) | (w >> 24);
}

/*
 Implementation of trasnformation for key schedule.
 How it works:
    subword:
        1. Splits the 32-bit into four bytes
        2. Applies S-box substitution to each byte
        3. Returns the substituted bytes combined into a new 32-bit word

    rotword:
        1. Rotates the 32-bit word by one byte (first byte becomes last)
 */
static uint32_t subword(uint32_t w) {
    return ((uint32_t)s_box[w >> 24] << 24) |
           ((uint32_t)s_box[(w >> 16) & 0xff] << 16) |
           ((uint32_t)s_box[(w >> 8) & 0xff] << 8) |
            (uint32_t)s_box[w & 0xff];
}

/*
 Applies the inverse MixColumns transformation to a single 32-bit word
 How it works:
    1. Spplit 32-bit input into four bytes
    2. Compute each new byte with finite-field and XORs
    3. Combine the new words into one 32-bit word
 */
static uint32_t inv_mix_column_word(uint32_t w) {
    uint8_t b0 = w >> 24;
    uint8_t b1 = (w >> 16) & 0xFF;
    uint8_t b2 = (w >> 8) & 0xFF;
    uint8_t b3 = w & 0xFF;

    uint8_t r0 = multi14(b0) ^ multi11(b1) ^ multi13(b2) ^ multi9(b3);
    uint8_t r1 = multi9(b0)  ^ multi14(b1) ^ multi11(b2) ^ multi13(b3);
    uint8_t r2 = multi13(b0) ^ multi9(b1)  ^ multi14(b2) ^ multi11(b3);
    uint8_t r3 = multi11(b0) ^ multi13(b1) ^ multi9(b2)  ^ multi14(b3);

    return ((uint32_t)r0 << 24) |
           ((uint32_t)r1 << 16) |
           ((uint32_t)r2 << 8)  |
           ((uint32_t)r3);
}

int decrypt(
    const uint32_t key_schedules[44],
    const uint32_t t_tables[4][256],
    const uint8_t ciphertext[16],
    uint8_t plaintext[16]
) {
    uint32_t state[4];

    for (int i = 0; i < 4; i++) {
        state[i] =
            ((uint32_t)ciphertext[i * 4 + 0] << 24) |
            ((uint32_t)ciphertext[i * 4 + 1] << 16) |
            ((uint32_t)ciphertext[i * 4 + 2] << 8)  |
            ((uint32_t)ciphertext[i * 4 + 3]);

        state[i] ^= key_schedules[i];
    }

    for (int round = 1; round < 10; round++) {
        uint32_t temp[4];

        temp[0] =
            t_tables[0][(state[0] >> 24) & 0xFF] ^
            t_tables[1][(state[3] >> 16) & 0xFF] ^
            t_tables[2][(state[2] >> 8)  & 0xFF] ^
            t_tables[3][(state[1])       & 0xFF] ^
            key_schedules[round * 4 + 0];

        temp[1] =
            t_tables[0][(state[1] >> 24) & 0xFF] ^
            t_tables[1][(state[0] >> 16) & 0xFF] ^
            t_tables[2][(state[3] >> 8)  & 0xFF] ^
            t_tables[3][(state[2])       & 0xFF] ^
            key_schedules[round * 4 + 1];

        temp[2] =
            t_tables[0][(state[2] >> 24) & 0xFF] ^
            t_tables[1][(state[1] >> 16) & 0xFF] ^
            t_tables[2][(state[0] >> 8)  & 0xFF] ^
            t_tables[3][(state[3])       & 0xFF] ^
            key_schedules[round * 4 + 2];

        temp[3] =
            t_tables[0][(state[3] >> 24) & 0xFF] ^
            t_tables[1][(state[2] >> 16) & 0xFF] ^
            t_tables[2][(state[1] >> 8)  & 0xFF] ^
            t_tables[3][(state[0])       & 0xFF] ^
            key_schedules[round * 4 + 3];

        for (int i = 0; i < 4; i++) {
            state[i] = temp[i];
        }
    }

    uint32_t temp[4];

    temp[0] =
        ((uint32_t)inverse_s_box[(state[0] >> 24) & 0xFF] << 24) ^
        ((uint32_t)inverse_s_box[(state[3] >> 16) & 0xFF] << 16) ^
        ((uint32_t)inverse_s_box[(state[2] >> 8)  & 0xFF] << 8)  ^
        ((uint32_t)inverse_s_box[(state[1])       & 0xFF]) ^
        key_schedules[40];

    temp[1] =
        ((uint32_t)inverse_s_box[(state[1] >> 24) & 0xFF] << 24) ^
        ((uint32_t)inverse_s_box[(state[0] >> 16) & 0xFF] << 16) ^
        ((uint32_t)inverse_s_box[(state[3] >> 8)  & 0xFF] << 8)  ^
        ((uint32_t)inverse_s_box[(state[2])       & 0xFF]) ^
        key_schedules[41];

    temp[2] =
        ((uint32_t)inverse_s_box[(state[2] >> 24) & 0xFF] << 24) ^
        ((uint32_t)inverse_s_box[(state[1] >> 16) & 0xFF] << 16) ^
        ((uint32_t)inverse_s_box[(state[0] >> 8)  & 0xFF] << 8)  ^
        ((uint32_t)inverse_s_box[(state[3])       & 0xFF]) ^
        key_schedules[42];

    temp[3] =
        ((uint32_t)inverse_s_box[(state[3] >> 24) & 0xFF] << 24) ^
        ((uint32_t)inverse_s_box[(state[2] >> 16) & 0xFF] << 16) ^
        ((uint32_t)inverse_s_box[(state[1] >> 8)  & 0xFF] << 8)  ^
        ((uint32_t)inverse_s_box[(state[0])       & 0xFF]) ^
        key_schedules[43];

    for (int i = 0; i < 4; i++) {
        plaintext[i * 4 + 0] = temp[i] >> 24;
        plaintext[i * 4 + 1] = temp[i] >> 16;
        plaintext[i * 4 + 2] = temp[i] >> 8;
        plaintext[i * 4 + 3] = temp[i];
    }

    return 0;
}

int encrypt(
    const uint32_t key_schedules[44],
    const uint32_t t_tables[4][256],
    const uint8_t plaintext[16],
    uint8_t ciphertext[16]
) {
    uint32_t state[4];

    for (int i = 0; i < 4; i++) {
        state[i] =
            ((uint32_t)plaintext[i * 4 + 0] << 24) |
            ((uint32_t)plaintext[i * 4 + 1] << 16) |
            ((uint32_t)plaintext[i * 4 + 2] << 8)  |
            ((uint32_t)plaintext[i * 4 + 3]);

        state[i] ^= key_schedules[i];
    }

    for (int round = 1; round < 10; round++) {
        uint32_t temp_state[4];

        temp_state[0] =
            t_tables[0][(state[0] >> 24) & 0xFF]  ^
            t_tables[1][(state[1] >> 16) & 0xFF]  ^
            t_tables[2][(state[2] >> 8)  & 0xFF]  ^
            t_tables[3][(state[3])       & 0xFF]  ^
            key_schedules[round * 4 + 0];

        temp_state[1] =
            t_tables[0][(state[1] >> 24)  & 0xFF]  ^
            t_tables[1][(state[2] >> 16)  & 0xFF]  ^
            t_tables[2][(state[3] >> 8)   & 0xFF]  ^
            t_tables[3][(state[0])        & 0xFF]  ^
            key_schedules[round * 4 + 1];

        temp_state[2] =
            t_tables[0][(state[2] >> 24)  & 0xFF]  ^
            t_tables[1][(state[3] >> 16)  & 0xFF]  ^
            t_tables[2][(state[0] >> 8)   & 0xFF]   ^
            t_tables[3][(state[1]) & 0xFF]        ^
            key_schedules[round * 4 + 2];

        temp_state[3] =
            t_tables[0][(state[3] >> 24) & 0xFF]  ^
            t_tables[1][(state[0] >> 16) & 0xFF]  ^
            t_tables[2][(state[1] >> 8)  & 0xFF]   ^
            t_tables[3][(state[2])  & 0xFF]        ^
            key_schedules[round * 4 + 3];

        for (int i = 0; i < 4; i++) {
            state[i] = temp_state[i];
        }
    }

    uint32_t temp_state[4];

    temp_state[0] =
        ((uint32_t)s_box[(state[0] >> 24) & 0xFF] << 24) ^
        ((uint32_t)s_box[(state[1] >> 16) & 0xFF] << 16) ^
        ((uint32_t)s_box[(state[2] >> 8)  & 0xFF] << 8)  ^
        ((uint32_t)s_box[(state[3])       & 0xFF]) ^
        key_schedules[40];

    temp_state[1] =
        ((uint32_t)s_box[(state[1] >> 24) & 0xFF] << 24) ^
        ((uint32_t)s_box[(state[2] >> 16) & 0xFF] << 16) ^
        ((uint32_t)s_box[(state[3] >> 8)  & 0xFF] << 8)  ^
        ((uint32_t)s_box[(state[0])       & 0xFF]) ^
        key_schedules[41];

    temp_state[2] =
        ((uint32_t)s_box[(state[2] >> 24) & 0xFF] << 24) ^
        ((uint32_t)s_box[(state[3] >> 16) & 0xFF] << 16) ^
        ((uint32_t)s_box[(state[0] >> 8)  & 0xFF] << 8)  ^
        ((uint32_t)s_box[(state[1])       & 0xFF]) ^
        key_schedules[42];

    temp_state[3] =
        ((uint32_t)s_box[(state[3] >> 24) & 0xFF] << 24) ^
        ((uint32_t)s_box[(state[0] >> 16) & 0xFF] << 16) ^
        ((uint32_t)s_box[(state[1] >> 8)  & 0xFF] << 8)  ^
        ((uint32_t)s_box[(state[2])       & 0xFF]) ^
        key_schedules[43];

    for (int i = 0; i < 4; i++) {
        state[i] = temp_state[i];
    }

    for (int i = 0; i < 4; i++) {
        ciphertext[i * 4 + 0] = state[i] >> 24;
        ciphertext[i * 4 + 1] = state[i] >> 16;
        ciphertext[i * 4 + 2] = state[i] >> 8;
        ciphertext[i * 4 + 3] = state[i];
    }

    return 0;
}

void generate_inverse_tables(uint32_t tables[4][256]) {
    for (int i = 0; i < 256; i++) {
        uint8_t inv = inverse_s_box[i];

        tables[0][i] =
            (multi14(inv) << 24) ^
            (multi9(inv)  << 16) ^
            (multi13(inv) << 8)  ^
            (multi11(inv));

        tables[1][i] =
            (multi11(inv) << 24) ^
            (multi14(inv) << 16) ^
            (multi9(inv)  << 8)  ^
            (multi13(inv));

        tables[2][i] =
            (multi13(inv) << 24) ^
            (multi11(inv) << 16) ^
            (multi14(inv) << 8)  ^
            (multi9(inv));

        tables[3][i] =
            (multi9(inv)  << 24) ^
            (multi13(inv) << 16) ^
            (multi11(inv) << 8)  ^
            (multi14(inv));

    }
}

void generate_tables(uint32_t tables[4][256]) {
    for (int i = 0; i < 256; i++) {
        const uint8_t s = s_box[i];
        const uint8_t s2 = xtime(s);
        const uint8_t s3 = mul3(s);

        tables[0][i] = (s2 << 24) | (s  << 16) | (s  << 8) | s3;
        tables[1][i] = (s3 << 24) | (s2 << 16) | (s  << 8) | s;
        tables[2][i] = (s  << 24) | (s3 << 16) | (s2 << 8) | s;
        tables[3][i] = (s  << 24) | (s  << 16) | (s3 << 8) | s2;

    }
}

/*
 Expands a 128-bit key into 44 32-bit round keys.
 How it works:
    1. Creates 4 words from the 16 byte key
    2. For words 5 to 44 take previous word
    If 'i' is multiple of 4:
        3. Rotate the word by one byte to the left (rotword)
        4. Apply S-box to each byte (subword)
        5. XOR the result with RCON
    Else:
        3. Use temp directly without modification
        4. XOR temp with the word 4 positions earlier
 */
void key_schedule(const uint8_t *key, uint32_t *roundkey) {
    for (int i = 0; i < 4; i++) {
        roundkey[i] =
        ((uint32_t)key[4*i] << 24) |
        ((uint32_t)key[4*i + 1] << 16) |
        ((uint32_t)key[4*i + 2] << 8) |
        (uint32_t)key[4*i + 3];
    }

    for (int i = 4; i < 44; i++) {
        uint32_t temp = roundkey[i-1];
        if (i % 4 == 0) {
            temp = subword(rotword(temp)) ^ (RCON[i/4 - 1] << 24);
        }
        roundkey[i] = roundkey[i-4] ^ temp;
    }
}

/*
 Prepares the round key for decryption.
 How it works:
    1. Reverse the order of round keys (round-by-round)
    2. Apply InverseMixColumns to rounds 1-9
    3. Keys 0 and 10 are left unchanged
 */
void reverse_round_keys(uint32_t *rk, uint32_t *inv_rk) {
    for (int round = 0; round < 11; round++) {
        for (int i = 0; i < 4; i++) {
            inv_rk[round * 4 + i] =
                rk[(10 - round) * 4 + i];
        }
    }

    for (int round = 1; round < 10; round++) {
        for (int i = 0; i < 4; i++) {
            inv_rk[round * 4 + i] =
                inv_mix_column_word(inv_rk[round * 4 + i]);
        }
    }
}

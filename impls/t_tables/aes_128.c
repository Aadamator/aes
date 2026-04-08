#include <stdint.h>

#include "aes_128.h"

#include "s_box.h"

static const uint8_t RCON[10] ={
    0x01,0x02,0x04,0x08,
    0x10,0x20,0x40,0x80,
    0x1B,0x36
};

static uint8_t xtime(uint8_t x) {
    return (x << 1) ^ ((x & 0x80) ? 0x1B : 0x00);
}

static uint8_t mul3(uint8_t x) {
    return xtime(x) ^ x;
}

static uint32_t rotword(uint32_t w) {
    return (w << 8) | (w >> 24);
}

static uint32_t subword(uint32_t w) {
    return ((uint32_t)s_box[w >> 24] << 24) |
           ((uint32_t)s_box[(w >> 16) & 0xff] << 16) |
           ((uint32_t)s_box[(w >> 8) & 0xff] << 8) |
            (uint32_t)s_box[w & 0xff];
}

int decrypt(
    const uint32_t key_schedules[44],
    const uint32_t t_tables[4][256],
    const uint8_t ciphertext[16],
    uint8_t plaintext[16]
) {
    // placeholder, just returns the cipertext
    for (int i = 0; i < 16; ++i) plaintext[i] = ciphertext[i];

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

#include <stdint.h>
#include <string.h>

#include "s_box.h"

static const uint8_t round_constants[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36,
};

static void rot_word(uint8_t *word) {
    const uint8_t tmp = word[0];

    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = tmp;
}


static void sub_word(uint8_t *word) {
    for (int i = 0; i < 4; i++) {
        word[i] = s_box[word[i]];
    }
}

void key_expansion(const uint8_t cipherkey[16], uint8_t key_schedules[11][16]) {
    // copy cipher key to the first position
    memcpy(key_schedules[0], cipherkey, 16);

    for (int round = 1; round < 11; round++) {
        uint8_t temp[4];

        memcpy(temp, &key_schedules[round - 1][12], 4);

        rot_word(temp);
        sub_word(temp);

        temp[0] ^= round_constants[round - 1];

        // key_schedules[i] = key_schedules[i-1] xor (transformed word || 0)
        for (int j = 0; j < 4; j++) {
            key_schedules[round][j] = key_schedules[round - 1][j] ^ temp[j];
        }

        for (int j = 4; j < 16; j++) {
            key_schedules[round][j] = key_schedules[round - 1][j] ^ key_schedules[round][j - 4];
        }
    }
}

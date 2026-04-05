#include <stdint.h>
#include <string.h>

#include "constants.h"
#include "s_box.h"

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

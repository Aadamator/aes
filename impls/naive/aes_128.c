#include <stdint.h>
#include <string.h>

#include "aes_128.h"
#include "constants.h"
#include "state_utils.h"

static uint8_t xtime(uint8_t x) {
    return (x << 1) ^ (((x >> 7) & 1) * 0x1b);
}

void add_round_key(const uint8_t round_key[16], uint8_t state[16]) {
    for (int i = 0; i < 16; i++) {
        state[i] ^= round_key[i];
    }
}

int encrypt(const uint8_t key[16], const uint8_t plaintext[16], uint8_t ciphertext[16]) {
    const int direction = 0;
    uint8_t state[16];
    uint8_t key_schedules[11][16];

    // initialize state with plaintext
    memcpy(state, plaintext, 16);

    // create key schedules
    key_expansion(key, key_schedules);

    // begin with a key addition
    add_round_key(key_schedules[0], state);

    // rounds 1-9 are ordinary rounds
    for (int round = 1; round < 10; round++) {
        sub_bytes(state);
        shift_rows(direction, state);
        mix_columns(state);
        add_round_key(key_schedules[round], state);
    }

    // last round is special: there is no mix_columns
    sub_bytes(state);
    shift_rows(direction, state);
    add_round_key(key_schedules[10], state);

    // copy the state to the ciphertext
    memcpy(ciphertext, state, 16);

    return 0;
}

void key_expansion(const uint8_t cipherkey[16], uint8_t key_schedules[11][16]) {
    // Copy cipher key to K0
    memcpy(key_schedules[0], cipherkey, 16);

    for (int i = 1; i < 11; i++) {
        uint8_t temp[4];
        memcpy(temp, key_schedules[i-1] + 12, 4);  // Last column

        // RotWord
        uint8_t t = temp[0];
        temp[0] = temp[1]; temp[1] = temp[2]; temp[2] = temp[3]; temp[3] = t;

        // SubWord
        temp[0] = s_box[temp[0]];
        temp[1] = s_box[temp[1]];
        temp[2] = s_box[temp[2]];
        temp[3] = s_box[temp[3]];

        // XOR Rcon
        temp[0] ^= round_constants[i-1];

        // K[i] = K[i-1] XOR (transformed word || 0)
        for (int j = 0; j < 4; j++) {
            key_schedules[i][j] = key_schedules[i-1][j] ^ temp[j];
        }
        for (int j = 4; j < 16; j++) {
            key_schedules[i][j] = key_schedules[i-1][j] ^ key_schedules[i][j-4];
        }
    }
}

void mix_columns(uint8_t state[16]) {
    for (int col = 0; col < 4; col++) {
        uint8_t col_state[4] = {state[col*4], state[col*4+1], state[col*4+2], state[col*4+3]};
        uint8_t col_out[4];

        // Mc matrix: {2,3,1,1}, {1,2,3,1}, {1,1,2,3}, {3,1,1,2}
        col_out[0] = xtime(col_state[0]) ^ xtime(col_state[1]) ^ col_state[1] ^ col_state[2] ^ col_state[3];
        col_out[1] = col_state[0] ^ xtime(col_state[1]) ^ xtime(col_state[2]) ^ col_state[2] ^ col_state[3];
        col_out[2] = col_state[0] ^ col_state[1] ^ xtime(col_state[2]) ^ xtime(col_state[3]) ^ col_state[3];
        col_out[3] = xtime(col_state[0]) ^ col_state[0] ^ col_state[1] ^ col_state[2] ^ xtime(col_state[3]);

        state[col*4+0] = col_out[0];
        state[col*4+1] = col_out[1];
        state[col*4+2] = col_out[2];
        state[col*4+3] = col_out[3];
    }
}

void shift_rows(const int direction, uint8_t state[16]) {
    const int nb = 4; // Nb = 4 (number of columns) for 16-byte (128-bit)
    const uint8_t shifts[4] = {0, 1, 2, 3}; // Nb = 4 (AES-128)
    int i;
    int j;
    uint8_t matrix[4][4];
    uint8_t tmp[nb];

    map_from_bytes(state, matrix);

    // encryption (direction=0)
    if (direction == 0) {
        for (i = 1; i < 4; i++) {
            for (j = 0; j < nb; j++) {
                tmp[j] = matrix[i][(j + shifts[i]) % nb];
            }

            for (j = 0; j < nb; j++) {
                matrix[i][j] = tmp[j];
            }
        }
    } else {
        for (i = 1; i < 4; i++) {
            for (j = 0; j < nb; j++) {
                tmp[j] = matrix[i][(nb + j - shifts[i]) % nb];
            }

            for (j = 0; j < nb; j++) {
                matrix[i][j] = tmp[j];
            }
        }
    }

    // back to flat state
    map_to_bytes(matrix, state);
}

void sub_bytes(uint8_t state[16]) {
    for (int i = 0; i < 16; i++) {
        state[i] = s_box[state[i]];
    }
}

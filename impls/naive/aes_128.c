#include <stdint.h>

#include "aes_128.h"

#include "gf.h"
#include "matrix.h"
#include "s_box.h"

static const int nb = 4;  // Nb = 4 (number of columns) for 16-byte (128-bit)
static const uint8_t shifts[4] = {0, 1, 2, 3}; // Nb = 4 (AES-128)

/**
 * @brief Performs the lookup but takes an S-box as reference, i.e. either the normal or the
 * inverse.
 *
 * @param s_box_ref     256-byte S-box look-up table.
 * @param state         4x4 matrix representing the current state of the 16-byte ciphertext.
 */
static void _sub_bytes(const uint8_t s_box_ref[256], uint8_t state[4][4]) {
    for(int i = 0; i < 4; i++){
        state[i][0] = s_box_ref[state[i][0]];
        state[i][1] = s_box_ref[state[i][1]];
        state[i][2] = s_box_ref[state[i][2]];
        state[i][3] = s_box_ref[state[i][3]];
    }
}

void add_round_key(const uint8_t round_key[16], uint8_t state[4][4]) {
    uint8_t round_key_matrix[4][4] = {0};

    // convert the round key to a matrix form to easily map
    matrix_from_array(round_key, round_key_matrix);

    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            state[row][col] ^= round_key_matrix[row][col];
        }
    }
}

int decrypt(const uint8_t key_schedules[11][16], const uint8_t ciphertext[16], uint8_t plaintext[16]) {
    uint8_t state[4][4] = {0};

    // initialize state with ciphertext
    matrix_from_array(ciphertext, state);

    // the first round is special: without inverse_mix_columns
    add_round_key(key_schedules[10], state);
    inverse_sub_bytes(state);
    inverse_shift_rows(state);

    // rounds 2-10 are ordinary rounds
    for (int round = 9; round > 0; --round) {
        add_round_key(key_schedules[round], state);
        inverse_mix_columns(state);
        inverse_sub_bytes(state);
        inverse_shift_rows(state);
    }

    // end with the extra key addition
    add_round_key(key_schedules[0], state);

    // flatten the state to the 16-byte plaintext block
    matrix_to_array(state, plaintext);

    return 0;
}

int encrypt(const uint8_t key_schedules[11][16], const uint8_t plaintext[16], uint8_t ciphertext[16]) {
    uint8_t state[4][4] = {0};

    // initialize state with plaintext
    matrix_from_array(plaintext, state);

    // begin with a key addition
    add_round_key(key_schedules[0], state);

    // rounds 1-9 are ordinary rounds
    for (int round = 1; round < 10; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(key_schedules[round], state);
    }

    // last round is special: there is no mix_columns
    sub_bytes(state);
    shift_rows(state);
    add_round_key(key_schedules[10], state);

    // flatten the state to the 16-byte ciphertext block
    matrix_to_array(state, ciphertext);

    return 0;
}

void inverse_mix_columns(uint8_t state[4][4]) {
    int i;
    int j;
    uint8_t b[4][4] = {0};

    for(j = 0; j < nb; j++) {
        for(i = 0; i < 4; i++) {
            b[i][j] = gf_multiply(0xe,state[i][j])
                ^ gf_multiply(0xb,state[(i + 1) % 4][j])
                ^ gf_multiply(0xd,state[(i + 2) % 4][j])
                ^ gf_multiply(0x9,state[(i + 3) % 4][j]);
        }
    }

    for(i = 0; i < 4; i++) {
        for(j = 0; j < nb; j++) {
            state[i][j] = b[i][j];
        }
    }
}

void inverse_shift_rows(uint8_t state[4][4]) {
    int j;
    uint8_t tmp[nb];

    for (int i = 1; i < 4; i++) {
        for (j = 0; j < nb; j++) {
            tmp[j] = state[i][(nb + j - shifts[i]) % nb];
        }

        for (j = 0; j < nb; j++) {
            state[i][j] = tmp[j];
        }
    }
}

void inverse_sub_bytes(uint8_t state[4][4]) {
    _sub_bytes(inverse_s_box, state);
}

void mix_columns(uint8_t state[4][4]) {
    int i;
    int j;
    uint8_t b[4][4] = {0};

    for(j = 0; j < nb; j++) {
        for(i = 0; i < 4; i++) {
            b[i][j] = gf_multiply(2, state[i][j])
                ^ gf_multiply(3,state[(i + 1) % 4][j])
                ^ state[(i + 2) % 4][j]
                ^ state[(i + 3) % 4][j];
        }
    }

    for(i = 0; i < 4; i++) {
        for(j = 0; j < nb; j++) {
            state[i][j] = b[i][j];
        }
    }
}

void shift_rows(uint8_t state[4][4]) {
    int j;
    uint8_t tmp[nb];

    for (int i = 1; i < 4; i++) {
        for (j = 0; j < nb; j++) {
            tmp[j] = state[i][(j + shifts[i]) % nb];
        }

        for (j = 0; j < nb; j++) {
            state[i][j] = tmp[j];
        }
    }
}

void sub_bytes(uint8_t state[4][4]) {
    _sub_bytes(s_box, state);
}

#include <stdint.h>
#include <string.h>

#include "aes_128.h"

#include "gf.h"
#include "matrix.h"
#include "s_box.h"

static const uint8_t shifts[4] = {0, 1, 2, 3}; // Nb = 4 (AES-128)

void add_round_key(const uint8_t round_key[16], uint8_t state[16]) {
    for (int i = 0; i < 16; i++) {
        state[i] ^= round_key[i];
    }
}

int decrypt(const uint8_t key_schedules[11][16], const uint8_t ciphertext[16], uint8_t plaintext[16]) {
    uint8_t state[16];

    // initialize state with ciphertext
    memcpy(state, ciphertext, 16);

    // first round is special: without inverse_mix_columns
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

    // copy the state to the plaintext
    memcpy(plaintext, state, 16);

    return 0;
}

int encrypt(const uint8_t key_schedules[11][16], const uint8_t plaintext[16], uint8_t ciphertext[16]) {
    uint8_t state[16];

    // initialize state with plaintext
    memcpy(state, plaintext, 16);

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

    // copy the state to the ciphertext
    memcpy(ciphertext, state, 16);

    return 0;
}

void inverse_mix_columns(uint8_t state[16]) {
    const int nb = 4; // Nb = 4 (number of columns) for 16-byte (128-bit)
    int i;
    int j;
    uint8_t a[4][4] = {0};
    uint8_t b[4][4] = {0};

    matrix_from_array(state, a);

    for(j = 0; j < nb; j++) {
        for(i = 0; i < 4; i++) {
            b[i][j] = gf_multiply(0xe,a[i][j])
                ^ gf_multiply(0xb,a[(i + 1) % 4][j])
                ^ gf_multiply(0xd,a[(i + 2) % 4][j])
                ^ gf_multiply(0x9,a[(i + 3) % 4][j]);
        }
    }

    for(i = 0; i < 4; i++) {
        for(j = 0; j < nb; j++) {
            a[i][j] = b[i][j];
        }
    }

    // back to flat state
    matrix_to_array(a, state);
}

void inverse_shift_rows(uint8_t state[16]) {
    const int nb = 4; // Nb = 4 (number of columns) for 16-byte (128-bit)
    int j;
    uint8_t matrix[4][4];
    uint8_t tmp[nb];

    matrix_from_array(state, matrix);

    for (int i = 1; i < 4; i++) {
        for (j = 0; j < nb; j++) {
            tmp[j] = matrix[i][(nb + j - shifts[i]) % nb];
        }

        for (j = 0; j < nb; j++) {
            matrix[i][j] = tmp[j];
        }
    }

    // back to flat state
    matrix_to_array(matrix, state);
}

void mix_columns(uint8_t state[16]) {
    const int nb = 4; // Nb = 4 (number of columns) for 16-byte (128-bit)
    int i;
    int j;
    uint8_t a[4][4] = {0};
    uint8_t b[4][4] = {0};

    matrix_from_array(state, a);

    for(j = 0; j < nb; j++) {
        for(i = 0; i < 4; i++) {
            b[i][j] = gf_multiply(2, a[i][j])
                ^ gf_multiply(3,a[(i + 1) % 4][j])
                ^ a[(i + 2) % 4][j]
                ^ a[(i + 3) % 4][j];
        }
    }

    for(i = 0; i < 4; i++) {
        for(j = 0; j < nb; j++) {
            a[i][j] = b[i][j];
        }
    }

    // back to flat state
    matrix_to_array(a, state);
}

void shift_rows(uint8_t state[16]) {
    const int nb = 4; // Nb = 4 (number of columns) for 16-byte (128-bit)
    int j;
    uint8_t matrix[4][4];
    uint8_t tmp[nb];

    matrix_from_array(state, matrix);

    for (int i = 1; i < 4; i++) {
        for (j = 0; j < nb; j++) {
            tmp[j] = matrix[i][(j + shifts[i]) % nb];
        }

        for (j = 0; j < nb; j++) {
            matrix[i][j] = tmp[j];
        }
    }

    // back to flat state
    matrix_to_array(matrix, state);
}

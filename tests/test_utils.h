#pragma once

#define NUM_ROUNDS 10

typedef struct {
    char start[33];
    char s_box[33];
    char s_row[33];
    char m_col[33];
    char k_sch[33];
} RoundState;

extern const char ciphertext_as_hex[33];
extern const char key_as_hex[33];
extern const char plaintext_as_hex[33];

void assert_round_state(
    const uint8_t expected[16],
    const uint8_t actual[16],
    int round,
    const char *step
);

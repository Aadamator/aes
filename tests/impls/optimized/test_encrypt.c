#include <stdint.h>
#include <string.h>

#include "sodium.h"
#include "unity.h"

#include "impls/optimized/aes_128.h"
#include "keys.h"
#include "../../utilities/assertations.h"
#include "../../utilities/constants.h"
#include "../../utilities/macros.h"
#include "../../utilities/types.h"

/**
 * @brief Test for a single round of AES-128 encryption based on the test vectors in Appendix B. of
 * "The design of Rijndael: the advanced encryption standard (AES)".
 *
 * @param round         The current round.
 * @param key_schedules The expanded key schedules.
 * @param state         The current state of the 16-byte ciphertext.
 */
static void test_single_round(
    const int round,
    const uint8_t key_schedules[11][16],
    uint8_t state[16]
) {
    const RoundState *round_state = &round_states[round - 1];
    uint8_t expected[16] = {0};
    uint8_t key_schedule[16] = {0};

    // R[n].start
    sodium_hex2bin(
        expected,
        sizeof(expected),
        round_state->start,
        strlen(round_state->start),
        NULL, NULL, NULL);
    assert_round_state(expected, state, round, "start");

    // R[n].s_box
    sodium_hex2bin(
        expected,
        sizeof(expected),
        round_state->s_box,
        strlen(round_state->s_box),
        NULL, NULL, NULL);
    optimized_sub_bytes(state);
    assert_round_state(expected, state, round, "s_box");

    // R[n].s_row
    sodium_hex2bin(
        expected,
        sizeof(expected),
        round_state->s_row,
        strlen(round_state->s_row),
        NULL, NULL, NULL);
    shift_rows(state);
    assert_round_state(expected, state, round, "s_row");

    // R[n].m_col (only mix columns on rounds 1-9)
    if (round < NUM_ROUNDS) {
        sodium_hex2bin(
        expected,
        sizeof(expected),
        round_state->m_col,
        strlen(round_state->m_col),
        NULL, NULL, NULL);
        mix_columns(state);
        assert_round_state(expected, state, round , "m_col");
    }

    // R[n].k_sch
    memcpy(key_schedule, key_schedules[round], 16);
    sodium_hex2bin(
        expected,
        sizeof(expected),
        round_state->k_sch,
        strlen(round_state->k_sch),
        NULL, NULL, NULL);
    assert_round_state(expected, key_schedule, round, "k_sch");
    add_round_key(key_schedule, state);
}

/**
 * Test the full end-to-end functionality of AES-128 implementation using known plaintext ->
 * ciphertext
 */
static void test_encrypt(void) {
    uint8_t expected_ciphertext[16] = {0};
    uint8_t ciphertext[16] = {0};
    uint8_t key[16] = {0};
    uint8_t key_schedules[11][16] = {0}; // 16 bytes * 11 (0-10)
    uint8_t plaintext[16] = {0};

    sodium_hex2bin(
        expected_ciphertext,
        sizeof(expected_ciphertext),
        ciphertext_as_hex,
        strlen(ciphertext_as_hex),
        NULL, NULL, NULL);
    sodium_hex2bin(
        key,
        sizeof(key),
        key_as_hex,
        strlen(key_as_hex),
        NULL, NULL, NULL);
    sodium_hex2bin(
        plaintext,
        sizeof(plaintext),
        plaintext_as_hex,
        strlen(plaintext_as_hex),
        NULL, NULL, NULL);

    // pre-compute the round keys
    key_expansion(key, key_schedules);

    encrypt(key_schedules, plaintext, ciphertext);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_ciphertext, ciphertext, 16);
}

/**
 * Runs a test for each round and checks the state at each step of the encryption process.
 */
static void test_encrypt_rounds() {
    uint8_t start[16] = {0};
    uint8_t ciphertext[16] = {0};
    uint8_t expected[16] = {0};
    uint8_t key[16] = {0};
    uint8_t key_schedules[11][16] = {0}; // 16 bytes * 11 (0-10)
    uint8_t plaintext[16] = {0};
    uint8_t state[16] = {0};

    sodium_hex2bin(
        key,
        sizeof(key),
        key_as_hex,
        strlen(key_as_hex),
        NULL, NULL, NULL
    );
    sodium_hex2bin(
        plaintext,
        sizeof(plaintext),
        plaintext_as_hex,
        strlen(plaintext_as_hex),
        NULL, NULL, NULL
    );

    // initialize the state with the plaintext
    memcpy(state, plaintext, 16);

    // pre-compute the round keys
    key_expansion(key, key_schedules);

    // initialize for R[00] - before the rounds
    add_round_key(key_schedules[0], state);
    sodium_hex2bin(
        expected,
        16,
        round_states[0].start,
        32,
        NULL, NULL, NULL
    );
    assert_round_state(expected, state, 0, "start");

    // iterate each round updating the state
    for (int round = 1; round < NUM_ROUNDS + 1; round++) {
        test_single_round(round, key_schedules, state);
    }

    // verify
    sodium_hex2bin(
        expected,
        sizeof(expected),
        ciphertext_as_hex,
        strlen(ciphertext_as_hex),
        NULL, NULL, NULL
    );
    memcpy(ciphertext, state, 16);
    assert_round_state(expected, ciphertext, NUM_ROUNDS, "output");
}

// unity lifecycle functions
void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_encrypt);
    RUN_TEST(test_encrypt_rounds);

    return UNITY_END();
}

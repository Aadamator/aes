#include <string.h>

#include "aes_128.h"
#include "unity.h"
#include "sodium.h"
#include "test_utils.h"

static const RoundState round_states[NUM_ROUNDS] = {
    // R[01]
    {
        "193de3bea0f4e22b9ac68d2ae9f84808", // start
        "d42711aee0bf98f1b8b45de51e415230", // s_box
        "d4bf5d30e0b452aeb84111f11e2798e5", // s_row
        "046681e5e0cb199a48f8d37a2806264c", // m_col
        "a0fafe1788542cb123a339392a6c7605", // k_sch
    },
    // R[02]
    {
        "a49c7ff2689f352b6b5bea43026a5049",
        "49ded28945db96f17f39871a7702533b",
        "49db873b453953897f02d2f177de961a",
        "584dcaf11b4b5aacdbe7caa81b6bb0e5",
        "f2c295f27a96b9435935807a7359f67f",
    },
    // R[03]
    {
        "aa8f5f0361dde3ef82d24ad26832469a",
        "ac73cf7befc111df13b5d6b545235ab8",
        "acc1d6b8efb55a7b1323cfdf457311b5",
        "75ec0993200b633353c0cf7cbb25d0dc",
        "3d80477d4716fe3e1e237e446d7a883b",
    },
    // R[04]
    {
        "486c4eee671d9d0d4de3b138d65f58e7",
        "52502f2885a45ed7e311c807f6cf6a94",
        "52a4c89485116a28e3cf2fd7f6505e07",
        "0fd6daa9603138bf6fc0106b5eb31301",
        "ef44a541a8525b7fb671253bdb0bad00",
    },
    // R[05]
    {
        "e0927fe8c86363c0d9b1355085b8be01",
        "e14fd29be8fbfbba35c89653976cae7c",
        "e1fb967ce8c8ae9b356cd2ba974ffb53",
        "25d1a9adbd11d168b63a338e4c4cc0b0",
        "d4d1c6f87c839d87caf2b8bc11f915bc",
    },
    // R[06]
    {
        "f1006f55c1924cef7cc88b325db5d50c",
        "a163a8fc784f29df10e83d234cd503fe",
        "a14f3dfe78e803fc10d5a8df4c632923",
        "4b868d6d2c4a8980339df4e837d218d8",
        "6d88a37a110b3efddbf98641ca0093fd",
    },
    // R[07]
    {
        "260e2e173d41b77de86472a9fdd28b25",
        "f7ab31f02783a9ff9b4340d354b53d3f",
        "f783403f27433df09bb531ff54aba9d3",
        "1415b5bf461615ec274656d7342ad843",
        "4e54f70e5f5fc9f384a64fb24ea6dc4f",
    },
    // R[08]
    {
        "5a4142b11949dc1fa3e019657a8c040c",
        "be832cc8d43b86c00ae1d44dda64f2fe",
        "be3bd4fed4e1f2c80a642cc0da83864d",
        "00512fd1b1c889ff54766dcdfa1b99ea",
        "ead27321b58dbad2312bf5607f8d292f",
    },
    // R[09]
    {
        "ea835cf00445332d655d98ad8596b0c5",
        "87ec4a8cf26ec3d84d4c46959790e7a6",
        "876e46a6f24ce78c4d904ad897ecc395",
        "473794ed40d4e4a5a3703aa64c9f42bc",
        "ac7766f319fadc2128d12941575c006e",
    },
    // R[10]
    {
        "eb40f21e592e38848ba113e71bc342d2",
        "e9098972cb31075f3d327d94af2e2cb5",
        "e9317db5cb322c723d2e895faf090794",
        "", // no mix column for round 10
        "d014f9a8c9ee2589e13f0cc8b6630ca6",
    },
};

/**
 * Test for a single round of AES-128 encryption based on the test vectors in Appendix B. of "The
 * design of Rijndael: the advanced encryption standard (AES)".
 *
 * @param round         The current round.
 * @param key_schedules The expanded key schedules.
 * @param state         The current state of the AES-128 encryption.
 */
static void test_single_round(
    const int round,
    const uint8_t key_schedules[11][16],
    uint8_t state[16]
) {
    const RoundState *round_state = &round_states[round];
    uint8_t expected[16] = {0};
    uint8_t key_schedule[16] = {0};

    // R[n].start
    sodium_hex2bin(
        expected,
        sizeof(expected),
        round_state->start,
        strlen(round_state->start),
        NULL, NULL, NULL);
    assert_round_state(expected, state, round + 1, "start");

    // R[n].s_box
    sodium_hex2bin(
        expected,
        sizeof(expected),
        round_state->s_box,
        strlen(round_state->s_box),
        NULL, NULL, NULL);
    sub_bytes(state);
    assert_round_state(expected, state, round + 1, "s_box");

    // R[n].s_row
    sodium_hex2bin(
        expected,
        sizeof(expected),
        round_state->s_row,
        strlen(round_state->s_row),
        NULL, NULL, NULL);
    shift_rows(state);
    assert_round_state(expected, state, round + 1, "s_row");

    // R[n].m_col (only mix columns on rounds 1-9)
    if (round < NUM_ROUNDS - 1) {
        sodium_hex2bin(
        expected,
        sizeof(expected),
        round_state->m_col,
        strlen(round_state->m_col),
        NULL, NULL, NULL);
        mix_columns(state);
        assert_round_state(expected, state, round + 1, "m_col");
    }

    // R[n].k_sch
    memcpy(key_schedule, key_schedules[round + 1], 16);
    sodium_hex2bin(
        expected,
        sizeof(expected),
        round_state->k_sch,
        strlen(round_state->k_sch),
        NULL, NULL, NULL);
    assert_round_state(expected, key_schedule, round + 1, "k_sch");
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

    encrypt(key, plaintext, ciphertext);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_ciphertext, ciphertext, 16);
}

/**
 * Runs a test for each round and checks the state at each step of the encryption process.
 */
static void test_rounds() {
    uint8_t expected[16] = {0};
    uint8_t key[16] = {0};
    uint8_t round_keys[11][16]; // 16 bytes * 11 (0-10)
    uint8_t state[16] = {0};

    sodium_hex2bin(
        key,
        sizeof(key),
        key_as_hex,
        strlen(key_as_hex),
        NULL, NULL, NULL
    );
    // initialize the state with the plaintext
    sodium_hex2bin(
        state,
        sizeof(state),
        plaintext_as_hex,
        strlen(plaintext_as_hex),
        NULL, NULL, NULL
    );

    // get the round keys
    key_expansion(key, round_keys);

    // initialize for R[00] - before the rounds
    add_round_key(round_keys[0], state);
    sodium_hex2bin(
        expected,
        16,
        round_states[0].start,
        32,
        NULL, NULL, NULL
    );
    assert_round_state(expected, state, 0, "start");
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, state, 16);

    // iterate each round updating the state
    for (int i = 0; i < NUM_ROUNDS; i++) {
        test_single_round(i, round_keys, state);
    }

    // verify
    sodium_hex2bin(
        expected,
        sizeof(expected),
        ciphertext_as_hex,
        strlen(ciphertext_as_hex),
        NULL, NULL, NULL
    );
    assert_round_state(expected, state, NUM_ROUNDS, "output");
}

// unity lifecycle functions
void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_encrypt);
    RUN_TEST(test_rounds);

    return UNITY_END();
}

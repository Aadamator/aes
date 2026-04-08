#include <string.h>

#include "sodium.h"
#include "unity.h"

#include "impls/ni_instructions/aes_128.h"
#include "keys.h"
#include "matrix.h"
#include "s_box.h"
#include "../../utilities/assertations.h"
#include "../../utilities/constants.h"

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

// unity lifecycle functions
void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_encrypt);

    return UNITY_END();
}

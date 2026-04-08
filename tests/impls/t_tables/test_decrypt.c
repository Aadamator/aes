#include <stdint.h>
#include <string.h>

#include "sodium.h"
#include "unity.h"

#include "impls/t_tables/aes_128.h"
#include "../../utilities/constants.h"

/**
 * Test the full end-to-end functionality of AES-128 implementation using known ciphertext ->
 * plaintext
 */
static void test_decrypt(void) {
    uint8_t expected_plaintext[16] = {0};
    uint8_t ciphertext[16] = {0};
    uint8_t key[16] = {0};
    uint32_t key_schedules[44] = {0};
    uint8_t plaintext[16] = {0};
    uint32_t t_tables[4][256] = {0};

    sodium_hex2bin(
        ciphertext,
        sizeof(ciphertext),
        ciphertext_as_hex,
        strlen(ciphertext_as_hex),
        NULL, NULL, NULL);
    sodium_hex2bin(
        expected_plaintext,
        sizeof(expected_plaintext),
        plaintext_as_hex,
        strlen(plaintext_as_hex),
        NULL, NULL, NULL);
    sodium_hex2bin(
        key,
        sizeof(key),
        key_as_hex,
        strlen(key_as_hex),
        NULL, NULL, NULL);

    // pre-compute the round keys and t-tables
    key_schedule(key, key_schedules);
    generate_tables(t_tables);

    decrypt(key_schedules, t_tables, ciphertext, plaintext);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_plaintext, plaintext, 16);
}

// unity lifecycle functions
void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_decrypt);

    return UNITY_END();
}

#include <string.h>

#include "unity.h"
#include "aes_128.h"
#include "sodium.h"

void setUp(void) {}
void tearDown(void) {}

static void test_aes_128_encrypt(void) {
    // see pages 253-254 of "The Design of Rijndael"
    const char expected_ciphertext_as_hex[33] = "3925841d02dc09fbdc118597196a0b32";
    const char key_as_hex[33] = "2b7e151628aed2a6abf7158809cf4f3c";
    const char plaintext_as_hex[33] = "3243f6a8885a308d313198a2e0370734";

    uint8_t expected_ciphertext[16] = {0};
    uint8_t ciphertext[16] = {0};
    uint8_t key[16] = {0};
    uint8_t plaintext[16] = {0};

    sodium_hex2bin(
        expected_ciphertext,
        sizeof(expected_ciphertext),
        expected_ciphertext_as_hex,
        strlen(expected_ciphertext_as_hex),
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

    aes_128_encrypt(key, plaintext, ciphertext);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_ciphertext, ciphertext, 16);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_aes_128_encrypt);
    return UNITY_END();
}

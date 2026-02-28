#include "unity.h"
#include "aes_128.h"

void setUp(void) {}
void tearDown(void) {}

static void test_encrypt_zeroes(void) {
    uint8_t key[16] = {0}, plaintext[16] = {0}, ciphertext[16] = {0};

    aes_128_encrypt(key, plaintext, ciphertext);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(plaintext, ciphertext, 16);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_encrypt_zeroes);
    return UNITY_END();
}

#include "unity.h"

/**
 * Test the full end-to-end functionality of AES-128 implementation using known ciphertext ->
 * plaintext
 */
static void test_decrypt(void) {}

// unity lifecycle functions
void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_decrypt);

    return UNITY_END();
}

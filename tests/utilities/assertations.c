#include "unity.h"
#include "sodium.h"
#include "assertations.h"

void assert_round_state(
    const uint8_t expected[16],
    const uint8_t actual[16],
    const int round,
    const char *step
) {
    char actual_as_hex[33];
    char expected_as_hex[33];
    char message[128];

    sodium_bin2hex(
        actual_as_hex,
        sizeof(actual_as_hex),
        actual,
        16
    );
    sodium_bin2hex(
        expected_as_hex,
        sizeof(expected_as_hex),
        expected,
        16
    );

    snprintf(message, sizeof(message), "R[%d].%s mismatch: expected \"%s\", actual \"%s\"",
        round, step, expected_as_hex, actual_as_hex);
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, actual, 16, message);
}

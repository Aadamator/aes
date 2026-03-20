#pragma once

#include <stdint.h>

void assert_round_state(
    const uint8_t expected[16],
    const uint8_t actual[16],
    int round,
    const char *step
);

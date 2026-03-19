#include <stdint.h>

#include "constants.h"

uint8_t gf_multiply(const uint8_t a, const uint8_t b) {
    if (a && b) {
        return algo_table[(log_table[a] + log_table[b]) % 255];
    }

    return 0;
}

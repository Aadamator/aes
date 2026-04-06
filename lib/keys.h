#pragma once

#include <stdint.h>

/**
 * @brief Precompute the key schedules from the cipher key.
 *
 * @param cipherkey     16-byte cipher key.
 * @param key_schedules A list of 16-byte key schedules.
 */
void key_expansion(const uint8_t (*cipherkey)[16], uint8_t key_schedules[11][16]);

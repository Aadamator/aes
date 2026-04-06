#pragma once

#include <stdint.h>

extern const uint8_t inverse_s_box[256];
extern const uint8_t s_box[256];

/**
 * @brief Inverse of the sub bytes operation.
 *
 * @param state         16-byte current ciphertext.
 */
void inverse_sub_bytes(uint8_t (*state)[16]);

/**
 * @brief Substitute each byte of the state with another byte according to a lookup table.
 *
 * @param state         16-byte current ciphertext.
 */
void sub_bytes(uint8_t (*state)[16]);

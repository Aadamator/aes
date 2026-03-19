#pragma once

#include <stdint.h>

/**
 * @brief Galois Field GF(2^8) arithmetic that multiplies two elements in GF(2^8) using log/antilog
 * tables for O(1) lookup multiplication in the Rijndael field with irreducible polynomial m(x) =
 * x^8 + x^4 + x^3 + x + 1 (0x11B).
 *
 * @param a     The first GF(2^8) element to multiply.
 * @param b     The second GF(2^8) element to multiply.
 * @return      The product of a and b in GF(2^8), or 0 if either input is 0.
 */
uint8_t gf_multiply(uint8_t a, uint8_t b);

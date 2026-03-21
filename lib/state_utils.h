#pragma once

#include <stdint.h>

/**
 * @brief Maps a 4x4 matrix with the 16-bytes block. The ordering is column-major (down then
 * across).
 *
 * bytes[16]:
 *
 * [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15]
 *
 * matrix[4][4]:
 *
 * ┌─────┬─────┬─────┬─────┐
 * │ p0  │ p4  │ p8  │ p12 │
 * ├─────┼─────┼─────┼─────┤
 * │ p1  │ p5  │ p9  │ p13 │
 * ├─────┼─────┼─────┼─────┤
 * │ p2  │ p6  │ p10 │ p14 │
 * ├─────┼─────┼─────┼─────┤
 * │ p3  │ p7  │ p11 │ p15 │
 * └─────┴─────┴─────┴─────┘
 *
 * @param bytes     16-byte block to map to the matrix.
 * @param matrix    4x4 matrix to map from the bytes.
 */
void map_from_bytes(const uint8_t bytes[16], uint8_t matrix[4][4]);

/**
 * @brief Maps the contents of a 4x4 matrix to a 16-byte block.
 *
 * matrix[4][4]:
 *
 * ┌─────┬─────┬─────┬─────┐
 * │ p0  │ p4  │ p8  │ p12 │
 * ├─────┼─────┼─────┼─────┤
 * │ p1  │ p5  │ p9  │ p13 │
 * ├─────┼─────┼─────┼─────┤
 * │ p2  │ p6  │ p10 │ p14 │
 * ├─────┼─────┼─────┼─────┤
 * │ p3  │ p7  │ p11 │ p15 │
 * └─────┴─────┴─────┴─────┘
 *
 * bytes[16]:
 *
 * [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15]
 *
 * @param matrix    4x4 matrix to map to the bytes.
 * @param bytes     16-byte block to map from the matrix.
 */
void map_to_bytes(const uint8_t matrix[4][4], uint8_t bytes[16]);

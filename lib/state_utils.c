#include <stdint.h>

void map_from_bytes(const uint8_t bytes[16], uint8_t matrix[4][4]) {
    matrix[0][0] = bytes[0]; matrix[0][1] = bytes[4]; matrix[0][2] = bytes[8]; matrix[0][3] = bytes[12];
    matrix[1][0] = bytes[1]; matrix[1][1] = bytes[5]; matrix[1][2] = bytes[9]; matrix[1][3] = bytes[13];
    matrix[2][0] = bytes[2]; matrix[2][1] = bytes[6]; matrix[2][2] = bytes[10]; matrix[2][3] = bytes[14];
    matrix[3][0] = bytes[3]; matrix[3][1] = bytes[7]; matrix[3][2] = bytes[11]; matrix[3][3] = bytes[15];
}

void map_to_bytes(const uint8_t matrix[4][4], uint8_t bytes[16]) {
    bytes[0] = matrix[0][0];
    bytes[1] = matrix[1][0];
    bytes[2] = matrix[2][0];
    bytes[3] = matrix[3][0];
    bytes[4] = matrix[0][1];
    bytes[5] = matrix[1][1];
    bytes[6] = matrix[2][1];
    bytes[7] = matrix[3][1];
    bytes[8] = matrix[0][2];
    bytes[9] = matrix[1][2];
    bytes[10] = matrix[2][2];
    bytes[11] = matrix[3][2];
    bytes[12] = matrix[0][3];
    bytes[13] = matrix[1][3];
    bytes[14] = matrix[2][3];
    bytes[15] = matrix[3][3];
}

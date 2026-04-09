#include <stdint.h>

void matrix_from_array(const uint8_t array[16], uint8_t matrix[4][4]) {
    matrix[0][0] = array[0]; matrix[0][1] = array[4]; matrix[0][2] = array[8]; matrix[0][3] = array[12];
    matrix[1][0] = array[1]; matrix[1][1] = array[5]; matrix[1][2] = array[9]; matrix[1][3] = array[13];
    matrix[2][0] = array[2]; matrix[2][1] = array[6]; matrix[2][2] = array[10]; matrix[2][3] = array[14];
    matrix[3][0] = array[3]; matrix[3][1] = array[7]; matrix[3][2] = array[11]; matrix[3][3] = array[15];
}

void matrix_to_array(const uint8_t matrix[4][4], uint8_t array[16]) {
    array[0] = matrix[0][0];
    array[1] = matrix[1][0];
    array[2] = matrix[2][0];
    array[3] = matrix[3][0];
    array[4] = matrix[0][1];
    array[5] = matrix[1][1];
    array[6] = matrix[2][1];
    array[7] = matrix[3][1];
    array[8] = matrix[0][2];
    array[9] = matrix[1][2];
    array[10] = matrix[2][2];
    array[11] = matrix[3][2];
    array[12] = matrix[0][3];
    array[13] = matrix[1][3];
    array[14] = matrix[2][3];
    array[15] = matrix[3][3];
}

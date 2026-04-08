#include <stdint.h>
#include <string.h>
#include <stdio.h>

// --- CROSS-PLATFORM TIMING WRAPPER ---
#ifdef _WIN32
    #include <windows.h>
    typedef LARGE_INTEGER timer_t;
    void get_time(timer_t* t) { QueryPerformanceCounter(t); }
    double get_elapsed_ms(timer_t start, timer_t end) {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        return (double)(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
    }
#else
    #include <time.h>
    typedef struct timespec timer_t;
    void get_time(timer_t* t) { clock_gettime(CLOCK_MONOTONIC_RAW, t); }
    double get_elapsed_ms(timer_t start, timer_t end) {
        return (double)(end.tv_sec - start.tv_sec) * 1000.0 + (double)(end.tv_nsec - start.tv_nsec) / 1000000.0;
    }
#endif

#define ITERATIONS 1000000

static const uint8_t SBOX[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

// Math: Parallel xtime on 4 bytes inside a 32-bit word
static inline uint32_t xtime_word(uint32_t w) {
    uint32_t m = 0x80808080;
    uint32_t high_bits = w & m;
    return ((w & ~m) << 1) ^ ((high_bits >> 7) * 0x1b);
}

// Optimization: 4 XORs instead of 16
void add_round_key(uint32_t state[4], uint32_t roundKey[4]) {
    state[0] ^= roundKey[0];
    state[1] ^= roundKey[1];
    state[2] ^= roundKey[2];
    state[3] ^= roundKey[3];
}

// SubBytes: Unrolled byte access within the word
void sub_bytes(uint32_t state[4]) {
    for (int i = 0; i < 4; i++) {
        uint8_t* b = (uint8_t*)&state[i];
        b[0] = SBOX[b[0]]; b[1] = SBOX[b[1]]; b[2] = SBOX[b[2]]; b[3] = SBOX[b[3]];
    }
}

// ShiftRows: Bit-masking and re-packing bytes
// This logic handles the "column-major" requirement of AES using bit-manipulation
void shift_rows(uint32_t state[4]) {
    uint32_t new_state[4];
    // Column 0: B0, B5, B10, B15
    new_state[0] = (state[0] & 0x000000FF) | (state[1] & 0x0000FF00) | (state[2] & 0x00FF0000) | (state[3] & 0xFF000000);
    // Column 1: B4, B9, B14, B3
    new_state[1] = (state[1] & 0x000000FF) | (state[2] & 0x0000FF00) | (state[3] & 0x00FF0000) | (state[0] & 0xFF000000);
    // Column 2: B8, B13, B2, B7
    new_state[2] = (state[2] & 0x000000FF) | (state[3] & 0x0000FF00) | (state[0] & 0x00FF0000) | (state[1] & 0xFF000000);
    // Column 3: B12, B1, B6, B11
    new_state[3] = (state[3] & 0x000000FF) | (state[0] & 0x0000FF00) | (state[1] & 0x00FF0000) | (state[2] & 0xFF000000);
    
    state[0] = new_state[0]; state[1] = new_state[1]; state[2] = new_state[2]; state[3] = new_state[3];
}

// MixColumns: Word-based rotation math
#define ROTL8(x) (((x) << 8) | ((x) >> 24))

void mix_columns(uint32_t state[4]) {
    for (int i = 0; i < 4; i++) {
        uint32_t w = state[i];
        uint32_t x = xtime_word(w);
        uint32_t x_rot = ROTL8(x);
        uint32_t w_rot1 = ROTL8(w);
        uint32_t w_rot2 = ROTL8(w_rot1);
        uint32_t w_rot3 = ROTL8(w_rot2);

        state[i] = x ^ (x_rot ^ w_rot1) ^ w_rot2 ^ w_rot3;
    }
}

void aes_encrypt(uint32_t state[4], uint32_t roundKeys[11][4]) {
    add_round_key(state, roundKeys[0]);
    for (int r = 1; r < 10; r++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, roundKeys[r]);
    }
    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, roundKeys[10]);
}

int main() {
    // 128-bit state as 4 words
    uint32_t plaintext[4] = {0xA8F64332, 0x8D305A88, 0xA2983131, 0x340737E0}; 
    uint32_t state[4];
    uint32_t roundKeys[11][4] = {0}; // Simplified for demo

    timer_t start, end;
    printf("Starting Task 2 Word-Oriented Benchmark (%d iterations)...\n", ITERATIONS);

    get_time(&start);
    for (int i = 0; i < ITERATIONS; i++) {
        state[0] = plaintext[0]; state[1] = plaintext[1]; 
        state[2] = plaintext[2]; state[3] = plaintext[3];
        aes_encrypt(state, roundKeys);
    }
    get_time(&end);

    double ms = get_elapsed_ms(start, end);
    printf("Total: %.2f ms | Avg: %.2f ns/block\n", ms, (ms * 1000000.0) / ITERATIONS);

    return 0;
}
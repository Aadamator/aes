//
//  t_tables.c
//  aes_t_tables
//
//  Created by Adam Zvalo on 06/04/2026.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define ROTATE_BYTE(value) (((value & 0x00FFFFFF) << 8) | ((value & 0x00FFFFFF) >> 24))

uint32_t T0[256], T1[256], T2[256], T3[256];
uint32_t TD0[256], TD1[256], TD2[256], TD3[256];

static const uint8_t SBOX[256] = {
    0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
    0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
    0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
    0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
    0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
    0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
    0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
    0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
    0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
    0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
    0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
    0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
    0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
    0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
    0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
    0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};


static const uint8_t INV_SBOX[256] = {
    0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
    0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
    0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
    0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
    0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
    0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
    0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
    0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
    0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
    0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
    0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
    0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
    0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
    0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
    0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
    0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
};


static const uint8_t RCON[10] ={
    0x01,0x02,0x04,0x08,
    0x10,0x20,0x40,0x80,
    0x1B,0x36
};


static inline uint8_t xtime(uint8_t x) {
    return (x << 1) ^ ((x & 0x80) ? 0x1B : 0x00);
}

static inline uint8_t mul3(uint8_t x) {
    return xtime(x) ^ x;
}

static inline uint8_t multi9(uint8_t x) {
    return xtime(xtime(xtime(x))) ^ x;
}


static inline uint8_t multi11(uint8_t x) {
    return xtime(xtime(xtime(x))) ^ xtime(x) ^ x;
}


static inline uint8_t multi13(uint8_t x) {
    return xtime(xtime(xtime(x))) ^ xtime(xtime(x)) ^ x;
}


static inline uint8_t multi14(uint8_t x) {
    return xtime(xtime(xtime(x))) ^ xtime(xtime(x)) ^ xtime(x);
}


/*
 Generates lookup tables (T0-T3 and TD0-TD3) for both encryption and decryption.
 How it works:
    1. Iterates over all 256 byte values and applies S-box
    2. Computes its finite-field multiples
    3. Packs it into 32-bit words in different byte orders to populate the tables.
 */
void generate_tables(void) {
    for (int i = 0; i < 256; i++) {
        uint8_t s = SBOX[i];
        uint8_t s2 = xtime(s);
        uint8_t s3 = mul3(s);

        T0[i] = (s2 << 24) | (s  << 16) | (s  << 8) | s3;
        T1[i] = (s3 << 24) | (s2 << 16) | (s  << 8) | s;
        T2[i] = (s  << 24) | (s3 << 16) | (s2 << 8) | s;
        T3[i] = (s  << 24) | (s  << 16) | (s3 << 8) | s2;
        
        uint8_t inv = INV_SBOX[i];

        TD0[i] =
            (multi14(inv) << 24) ^
            (multi9(inv)  << 16) ^
            (multi13(inv) << 8)  ^
            (multi11(inv));

        TD1[i] =
            (multi11(inv) << 24) ^
            (multi14(inv) << 16) ^
            (multi9(inv)  << 8)  ^
            (multi13(inv));

        TD2[i] =
            (multi13(inv) << 24) ^
            (multi11(inv) << 16) ^
            (multi14(inv) << 8)  ^
            (multi9(inv));

        TD3[i] =
            (multi9(inv)  << 24) ^
            (multi13(inv) << 16) ^
            (multi11(inv) << 8)  ^
            (multi14(inv));
    }
}


/*
 Implementation of trasnformation for key schedule.
 How it works:
    subword:
        1. Splits the 32-bit into four bytes
        2. Applies S-box substitution to each byte
        3. Returns the substituted bytes combined into a new 32-bit word
 
    rotword:
        1. Rotates the 32-bit word by one byte (first byte becomes last)
 */
uint32_t subword(uint32_t w) {
    return ((uint32_t)SBOX[w >> 24] << 24) |
           ((uint32_t)SBOX[(w >> 16) & 0xff] << 16) |
           ((uint32_t)SBOX[(w >> 8) & 0xff] << 8) |
            (uint32_t)SBOX[w & 0xff];
}

uint32_t rotword(uint32_t w) {
    return (w << 8) | (w >> 24);
}


/*
 Expands a 128-bit key into 44 32-bit round keys.
 How it works:
    1. Creates 4 words from the 16 byte key
    2. For words 5 to 44 take previous word
    If 'i' is multiple of 4:
        3. Rotate the word by one byte to the left (rotword)
        4. Apply S-box to each byte (subword)
        5. XOR the result with RCON
    Else:
        3. Use temp directly without modification
        4. XOR temp with the word 4 positions earlier
 */
void key_schedule(uint32_t *roundkey, const uint8_t *key) {
    
    for (int i = 0; i < 4; i++) {
        roundkey[i] =
        ((uint32_t)key[4*i] << 24) |
        ((uint32_t)key[4*i + 1] << 16) |
        ((uint32_t)key[4*i + 2] << 8) |
        (uint32_t)key[4*i + 3];
    }
        
    for (int i = 4; i < 44; i++) {
        uint32_t temp = roundkey[i-1];
        if (i % 4 == 0) {
            temp = subword(rotword(temp)) ^ (RCON[i/4 - 1] << 24);
        }
        roundkey[i] = roundkey[i-4] ^ temp;
    }
}


/*
 Applies the inverse MixColumns transformation to a single 32-bit word
 How it works:
    1. Spplit 32-bit input into four bytes
    2. Compute each new byte with finite-field and XORs
    3. Combine the new words into one 32-bit word
 */
uint32_t inv_mix_column_word(uint32_t w) {
    uint8_t b0 = w >> 24;
    uint8_t b1 = (w >> 16) & 0xFF;
    uint8_t b2 = (w >> 8) & 0xFF;
    uint8_t b3 = w & 0xFF;

    uint8_t r0 = multi14(b0) ^ multi11(b1) ^ multi13(b2) ^ multi9(b3);
    uint8_t r1 = multi9(b0)  ^ multi14(b1) ^ multi11(b2) ^ multi13(b3);
    uint8_t r2 = multi13(b0) ^ multi9(b1)  ^ multi14(b2) ^ multi11(b3);
    uint8_t r3 = multi11(b0) ^ multi13(b1) ^ multi9(b2)  ^ multi14(b3);

    return ((uint32_t)r0 << 24) |
           ((uint32_t)r1 << 16) |
           ((uint32_t)r2 << 8)  |
           ((uint32_t)r3);
    }


/*
 Prepares the round key for decryption.
 How it works:
    1. Reverse the order of round keys (round-by-round)
    2. Apply InverseMixColumns to rounds 1-9
    3. Keys 0 and 10 are left unchanged
 */
void reverse_round_keys(uint32_t *rk, uint32_t *inv_rk) {
    for (int round = 0; round < 11; round++) {
        for (int i = 0; i < 4; i++) {
            inv_rk[round * 4 + i] =
                rk[(10 - round) * 4 + i];
        }
    }

    for (int round = 1; round < 10; round++) {
        for (int i = 0; i < 4; i++) {
            inv_rk[round * 4 + i] =
                inv_mix_column_word(inv_rk[round * 4 + i]);
        }
    }
}


/*
 Performs Encryption on a single 16-byte plaintext block.
 How it works:
    1. Load the 16-byte plaintext into four 4-byte words and apply the initial round key
    2. For round 1 to 9, transform the state using T-table lookups and XOR with the roundkey
    3. In the Final round apply SubBytes and ShiftRows via S-box lookup
 */
void aes_encrypt(uint32_t *roundkey, uint8_t *plaintext, uint8_t *output) {
    uint32_t state[4];
    
    
    for (int i = 0; i < 4; i++) {
        state[i] =
            ((uint32_t)plaintext[i * 4 + 0] << 24) |
            ((uint32_t)plaintext[i * 4 + 1] << 16) |
            ((uint32_t)plaintext[i * 4 + 2] << 8)  |
            ((uint32_t)plaintext[i * 4 + 3]);
        
        state[i] ^= roundkey[i];
    }
    
    for (int round = 1; round < 10; round++) {
        uint32_t temp_state[4];
        
        temp_state[0] =
            T0[(state[0] >> 24) & 0xFF]  ^
            T1[(state[1] >> 16) & 0xFF]  ^
            T2[(state[2] >> 8)  & 0xFF]  ^
            T3[(state[3])       & 0xFF]  ^
            roundkey[round * 4 + 0];
        
        temp_state[1] =
            T0[(state[1] >> 24)  & 0xFF]  ^
            T1[(state[2] >> 16)  & 0xFF]  ^
            T2[(state[3] >> 8)   & 0xFF]  ^
            T3[(state[0])        & 0xFF]  ^
            roundkey[round * 4 + 1];
        
        temp_state[2] =
            T0[(state[2] >> 24)  & 0xFF]   ^
            T1[(state[3] >> 16)  & 0xFF]   ^
            T2[(state[0] >> 8)   & 0xFF]   ^
            T3[(state[1]) & 0xFF]          ^
            roundkey[round * 4 + 2];
        
        temp_state[3] =
            T0[(state[3] >> 24) & 0xFF]   ^
            T1[(state[0] >> 16) & 0xFF]   ^
            T2[(state[1] >> 8)  & 0xFF]   ^
            T3[(state[2])  & 0xFF]        ^
            roundkey[round * 4 + 3];
        
        for (int i = 0; i < 4; i++) {
            state[i] = temp_state[i];
        }
    }
    
    uint32_t temp_state[4];
        
    temp_state[0] =
        ((uint32_t)SBOX[(state[0] >> 24) & 0xFF] << 24) ^
        ((uint32_t)SBOX[(state[1] >> 16) & 0xFF] << 16) ^
        ((uint32_t)SBOX[(state[2] >> 8)  & 0xFF] << 8)  ^
        ((uint32_t)SBOX[(state[3])       & 0xFF]) ^
        roundkey[40];

    temp_state[1] =
        ((uint32_t)SBOX[(state[1] >> 24) & 0xFF] << 24) ^
        ((uint32_t)SBOX[(state[2] >> 16) & 0xFF] << 16) ^
        ((uint32_t)SBOX[(state[3] >> 8)  & 0xFF] << 8)  ^
        ((uint32_t)SBOX[(state[0])       & 0xFF]) ^
        roundkey[41];

    temp_state[2] =
        ((uint32_t)SBOX[(state[2] >> 24) & 0xFF] << 24) ^
        ((uint32_t)SBOX[(state[3] >> 16) & 0xFF] << 16) ^
        ((uint32_t)SBOX[(state[0] >> 8)  & 0xFF] << 8)  ^
        ((uint32_t)SBOX[(state[1])       & 0xFF]) ^
        roundkey[42];

    temp_state[3] =
        ((uint32_t)SBOX[(state[3] >> 24) & 0xFF] << 24) ^
        ((uint32_t)SBOX[(state[0] >> 16) & 0xFF] << 16) ^
        ((uint32_t)SBOX[(state[1] >> 8)  & 0xFF] << 8)  ^
        ((uint32_t)SBOX[(state[2])       & 0xFF]) ^
        roundkey[43];

    for (int i = 0; i < 4; i++) {
        state[i] = temp_state[i];
    }
    
    for (int i = 0; i < 4; i++) {
        output[i * 4 + 0]  = state[i] >> 24;
        output[i * 4 + 1]  = state[i] >> 16;
        output[i * 4 + 2]  = state[i] >> 8;
        output[i * 4 + 3] = state[i];
    }
    
}


/*
 Performs Decryption on a single 16-byte plaintext block.
 How it works:
    1. Load the 16-byte ciphertext into four 4-byte words and apply the initial inverse round key
    2. For round 1 to 9, transform the state using inverse T-table lookups and XOR with the roundkey
    3. In the Final round, apply InvSubBytes and InvShiftRows via inverse S-box lookup
 */
void aes_decrypt(uint32_t *inv_roundkey, uint8_t *ciphertext, uint8_t *output) {
    uint32_t state[4];

    for (int i = 0; i < 4; i++) {
        state[i] =
            ((uint32_t)ciphertext[i * 4 + 0] << 24) |
            ((uint32_t)ciphertext[i * 4 + 1] << 16) |
            ((uint32_t)ciphertext[i * 4 + 2] << 8)  |
            ((uint32_t)ciphertext[i * 4 + 3]);

        state[i] ^= inv_roundkey[i];
    }

    for (int round = 1; round < 10; round++) {
        uint32_t temp[4];

        temp[0] =
            TD0[(state[0] >> 24) & 0xFF] ^
            TD1[(state[3] >> 16) & 0xFF] ^
            TD2[(state[2] >> 8)  & 0xFF] ^
            TD3[(state[1])       & 0xFF] ^
            inv_roundkey[round * 4 + 0];

        temp[1] =
            TD0[(state[1] >> 24) & 0xFF] ^
            TD1[(state[0] >> 16) & 0xFF] ^
            TD2[(state[3] >> 8)  & 0xFF] ^
            TD3[(state[2])       & 0xFF] ^
            inv_roundkey[round * 4 + 1];

        temp[2] =
            TD0[(state[2] >> 24) & 0xFF] ^
            TD1[(state[1] >> 16) & 0xFF] ^
            TD2[(state[0] >> 8)  & 0xFF] ^
            TD3[(state[3])       & 0xFF] ^
            inv_roundkey[round * 4 + 2];

        temp[3] =
            TD0[(state[3] >> 24) & 0xFF] ^
            TD1[(state[2] >> 16) & 0xFF] ^
            TD2[(state[1] >> 8)  & 0xFF] ^
            TD3[(state[0])       & 0xFF] ^
            inv_roundkey[round * 4 + 3];

        for (int i = 0; i < 4; i++) {
            state[i] = temp[i];
        }
    }

    uint32_t temp[4];

    temp[0] =
        ((uint32_t)INV_SBOX[(state[0] >> 24) & 0xFF] << 24) ^
        ((uint32_t)INV_SBOX[(state[3] >> 16) & 0xFF] << 16) ^
        ((uint32_t)INV_SBOX[(state[2] >> 8)  & 0xFF] << 8)  ^
        ((uint32_t)INV_SBOX[(state[1])       & 0xFF]) ^
        inv_roundkey[40];

    temp[1] =
        ((uint32_t)INV_SBOX[(state[1] >> 24) & 0xFF] << 24) ^
        ((uint32_t)INV_SBOX[(state[0] >> 16) & 0xFF] << 16) ^
        ((uint32_t)INV_SBOX[(state[3] >> 8)  & 0xFF] << 8)  ^
        ((uint32_t)INV_SBOX[(state[2])       & 0xFF]) ^
        inv_roundkey[41];

    temp[2] =
        ((uint32_t)INV_SBOX[(state[2] >> 24) & 0xFF] << 24) ^
        ((uint32_t)INV_SBOX[(state[1] >> 16) & 0xFF] << 16) ^
        ((uint32_t)INV_SBOX[(state[0] >> 8)  & 0xFF] << 8)  ^
        ((uint32_t)INV_SBOX[(state[3])       & 0xFF]) ^
        inv_roundkey[42];

    temp[3] =
        ((uint32_t)INV_SBOX[(state[3] >> 24) & 0xFF] << 24) ^
        ((uint32_t)INV_SBOX[(state[2] >> 16) & 0xFF] << 16) ^
        ((uint32_t)INV_SBOX[(state[1] >> 8)  & 0xFF] << 8)  ^
        ((uint32_t)INV_SBOX[(state[0])       & 0xFF]) ^
        inv_roundkey[43];

    for (int i = 0; i < 4; i++) {
        output[i * 4 + 0] = temp[i] >> 24;
        output[i * 4 + 1] = temp[i] >> 16;
        output[i * 4 + 2] = temp[i] >> 8;
        output[i * 4 + 3] = temp[i];
    }
}


int main(int argc, const char * argv[]) {
    uint8_t plaintext[16] = {
        0x00,0x11,0x22,0x33,
        0x44,0x55,0x66,0x77,
        0x88,0x99,0xaa,0xbb,
        0xcc,0xdd,0xee,0xff
    };
    
    uint8_t key[16] = {
        0x00,0x01,0x02,0x03,
        0x04,0x05,0x06,0x07,
        0x08,0x09,0x0a,0x0b,
        0x0c,0x0d,0x0e,0x0f
    };
    
    uint8_t ciphertext[16] = {
        0x69,0xC4,0xE0,0xD8,
        0x6A,0x7B,0x04,0x30,
        0xD8,0xCD,0xB7,0x80,
        0x70,0xB4,0xC5,0x5A
    };

    uint32_t roundkey[44];
    uint8_t output[16];
    uint8_t decrypted[16];
    uint32_t inv_roundkey[44];

    generate_tables();
    key_schedule(roundkey, key);
    aes_encrypt(roundkey, plaintext, output);
    reverse_round_keys(roundkey, inv_roundkey);
    aes_decrypt(inv_roundkey, output, decrypted);

    for (int i = 0; i < 16; i++) {
        printf("%u\t->\t%u\n", ciphertext[i], (uint8_t)output[i]);
    }
    printf("\n----------------------------------------------\n");
    
    for (int i = 0; i < 16; i++) {
        printf("%u\t->\t%u\n", plaintext[i], (uint8_t)decrypted[i]);
    }

    return 0;
}

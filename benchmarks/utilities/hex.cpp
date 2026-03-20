#include <cstdio>

#include "hex.h"

void hex_to_bytes(const char* hex, uint8_t* bytes) {
  for (int i = 0; i < 16; ++i) {
    sscanf(hex + 2 * i, "%2hhx", &bytes[i]);
  }
}

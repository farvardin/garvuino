#pragma once

#include "constants.h"

#define INLINE inline __attribute__((always_inline))

INLINE uint8_t low_byte(uint16_t x) {
  return x & 0xFF;
}

INLINE uint8_t high_byte(uint16_t x) {
  return x >> 8;
}

INLINE int8_t high_sbyte(int16_t x) {
  return x >> 8;
}

INLINE uint8_t hhigh_byte(__uint24 x) {
  return x >> 16;
}

#pragma once

#include "util/types.h"

#define BIT_SIZE(_s) (sizeof(_s)*8)

// get bit at index [starting from right side]
static inline bool bit_get(u8 *self, u8 i) { return (*self >> i) % 2 == 1; }
/* static inline bool bit_get(u8 *self, u8 i) { return (*self >> i) & 0x1; } */
// set bit at index [starting from right side]
static inline void bit_set(u8 *self, u8 i) { *self |= (1 << i); }
// unset bit at index [starting from right side]
static inline void bit_unset(u8 *self, u8 i) { *self &= ~(1 << i); }
// assign bit at index [starting from right side]
static inline void bit_assign(u8 *self, u8 i, u8 n) { return n == 0x00 ? bit_unset(self, i) : bit_set(self, i); }

// isolate bits in byte [starting from right side]
static inline u8 _bit_isolate(u8 *self, u8 from, u8 to) { return (*self << to) >> to >> from; }
// isolate bits in byte [starting from left side]
static inline u8 bit_isolate(u8 *self, u8 from, u8 to) { return _bit_isolate(self, BIT_SIZE(u8)-1 - to, BIT_SIZE(u8)-1 - from); }

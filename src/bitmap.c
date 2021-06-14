#include "bitmap.h"

#include <math.h>
#include <stdio.h>

bitmap_t set_bit(bitmap_t n, int p) {
  bitmap_t mask = ((bitmap_t) 1) << p;
  return (n | mask);
}

bitmap_t unset_bit(bitmap_t n, int p) {
  bitmap_t mask = ((bitmap_t) 1) << p;
  return (n & ~mask);
}

size_t get_rightmost_set_bit(bitmap_t n) {
  #ifndef __GNUC__
  size_t count = __builtin_ctzll(n) + 1;

  #else
  size_t count = log2(n & -n) + 1;

  #endif /* __GNUC__ */

  return count;
}

size_t get_set_bits_number(bitmap_t n) {
  #ifdef __GNUC__
  size_t count = __builtin_popcountll(n);

  #else
  size_t count = 0;
  while (n) {
    count += n & 1;
    n >>= 1;
  }

  #endif /* __GNUC__ */

  return count;
}

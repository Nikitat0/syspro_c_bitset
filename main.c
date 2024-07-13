#include "bitset.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

int main() {
  size_t amount[4] = {0};
  for (size_t i = 0; i < 4; i++)
    scanf("%zu", &amount[i]);

  unsigned int max = 0;
  BitSet s[4];
  for (size_t i = 0; i < 4; i++) {
    s[i] = new_bitset();
    for (size_t j = 0; j < amount[i]; j++) {
      unsigned int n;
      scanf("%u", &n);
      max = n > max ? n : max;
      bitset_add(&s[i], n);
    }
  }

  bitset_union(&s[0], s[1]);
  bitset_intersection(&s[0], s[2]);
  bitset_difference(&s[0], s[3]);

  bool first = true;
  for (unsigned int i = 0; i <= max; i++)
    if (bitset_contains(&s[0], i)) {
      printf(first ? "%u" : " %u", i);
      first = false;
    }
  printf("\n");

  for (size_t i = 0; i < 4; i++)
    bitset_free(&s[i]);
}

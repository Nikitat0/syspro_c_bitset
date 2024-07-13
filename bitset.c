#include "bitset.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define SEGMENT_SIZE sizeof(size_t)
#define SEGMENT_CAP (8 * SEGMENT_SIZE)
#define SEGMENT_BYTES(length) (length * SEGMENT_SIZE))
#define SEGMENT_INDEX(n) (n / SEGMENT_CAP)
#define SEGMENT_MASK(n) ((size_t) 1 << (n % SEGMENT_CAP))

inline static size_t* bitset_segment_of(const BitSet* restrict self, unsigned int n);

BitSet new_bitset() {
  return (BitSet){0};
}

bool bitset_contains(const BitSet* restrict self, unsigned int n) {
  size_t* segment = bitset_segment_of(self, n);
  return segment != NULL && (*segment & SEGMENT_MASK(n)) != 0;
}

void bitset_add(BitSet* restrict self, unsigned int n) {
  size_t required = SEGMENT_INDEX(n) + 1;
  if (required > self->length) {
    self->data = realloc(self->data, required * SEGMENT_SIZE);
    memset(self->data + self->length, 0, (required - self->length) * SEGMENT_SIZE);
    self->length = required;
  }

  size_t* segment = bitset_segment_of(self, n);
  *segment |= SEGMENT_MASK(n);
}

void bitset_remove(BitSet* restrict self, unsigned int n) {
  size_t* segment = bitset_segment_of(self, n);
  if (segment != NULL)
    *segment &= ~SEGMENT_MASK(n);
}

void bitset_union(BitSet* restrict self, const BitSet other) {
  for (size_t i = 0; i < self->length && i < other.length; i++)
    self->data[i] |= other.data[i];

  if (self->length < other.length) {
    size_t diff = other.length - self->length;
    self->data = realloc(self->data, other.length * SEGMENT_SIZE);
    memcpy(self->data + self->length, other.data + self->length, diff * SEGMENT_SIZE);
    self->length = other.length;
  }
}

void bitset_intersection(BitSet* restrict self, const BitSet other) {
  if (self->length > other.length) {
    self->data = realloc(self->data, other.length * SEGMENT_SIZE);
    self->length = other.length;
  }
  for (size_t i = 0; i < self->length; i++)
    self->data[i] &= other.data[i];
}

void bitset_difference(BitSet* restrict self, const BitSet other) {
  for (size_t i = 0; i < self->length && i < other.length; i++)
    self->data[i] &= ~other.data[i];
}

void bitset_free(BitSet* self) {
  free(self->data);
  *self = (BitSet){0};
}

inline static size_t* bitset_segment_of(const BitSet* restrict self, unsigned int n) {
  if (SEGMENT_INDEX(n) >= self->length)
    return NULL;
  return self->data + SEGMENT_INDEX(n);
}

#ifndef BITSET_H
#define BITSET_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  size_t length;
  size_t* restrict data;
} BitSet;

BitSet new_bitset();

bool bitset_contains(const BitSet* restrict self, unsigned int n);

void bitset_add(BitSet* restrict self, unsigned int n);
void bitset_remove(BitSet* restrict self, unsigned int n);

void bitset_union(BitSet* restrict self, const BitSet other);
void bitset_intersection(BitSet* restrict self, const BitSet other);
void bitset_difference(BitSet* restrict self, const BitSet other);

void bitset_free(BitSet* self);

#endif

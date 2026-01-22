#include "sort.h"
#include "utils.h"
#include <stdlib.h>

#define SHRINK_FACTOR 13

static inline int should_swap(int left, int right, int ascending) { return ascending ? left > right : left < right; }

static inline size_t calculate_next_gap(size_t current_gap) {
  size_t new_gap = (current_gap * 10) / SHRINK_FACTOR;
  return new_gap < 1 ? 1 : new_gap;
}

int *comb_sort_by_value(const int *arr, size_t size, int ascending) {
  int *copy = duplicate_vec1(arr, size);
  if (!copy) {
    return NULL;
  }
  comb_sort_by_pointer(copy, size, ascending);
  return copy;
}

void comb_sort_by_pointer(int *arr, size_t size, int ascending) {
  size_t gap = size;
  int swapped = 1;

  while (gap > 1 || swapped) {
    gap = calculate_next_gap(gap);
    swapped = 0;

    for (size_t i = 0; i + gap < size; i++) {
      int *left = arr + i;
      int *right = arr + i + gap;
      if (should_swap(*left, *right, ascending)) {
        int temp = *left;
        *left = *right;
        *right = temp;
        swapped = 1;
      }
    }
  }
}

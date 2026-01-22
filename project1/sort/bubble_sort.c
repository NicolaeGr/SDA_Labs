#include "sort.h"
#include "utils.h"
#include <stdlib.h>

static inline int should_swap(int left, int right, int ascending) { return ascending ? left > right : left < right; }

int *bubble_sort_by_value(const int *arr, size_t size, int ascending) {
  int *copy = duplicate_vec1(arr, size);
  if (!copy) {
    return NULL;
  }
  bubble_sort_by_pointer(copy, size, ascending);
  return copy;
}

void bubble_sort_by_pointer(int *arr, size_t size, int ascending) {
  for (size_t pass = 0; pass < size - 1; pass++) {
    for (size_t current = 0; current < size - pass - 1; current++) {
      int *left = arr + current;
      int *right = arr + current + 1;
      if (should_swap(*left, *right, ascending)) {
        int temp = *left;
        *left = *right;
        *right = temp;
      }
    }
  }
}

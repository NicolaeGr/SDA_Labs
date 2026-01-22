#include "sort.h"
#include "utils.h"
#include <stdlib.h>

static inline void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

static inline int is_better_choice(int candidate, int current_best, int ascending) {
  return ascending ? candidate < current_best : candidate > current_best;
}

int *selection_sort_by_value(const int *arr, size_t size, int ascending) {
  int *copy = duplicate_vec1(arr, size);
  if (!copy) {
    return NULL;
  }
  selection_sort_by_pointer(copy, size, ascending);
  return copy;
}

void selection_sort_by_pointer(int *arr, size_t size, int ascending) {
  for (size_t sorted_boundary = 0; sorted_boundary < size - 1; sorted_boundary++) {
    size_t extreme_idx = sorted_boundary;

    for (size_t current = sorted_boundary + 1; current < size; current++) {
      if (is_better_choice(*(arr + current), *(arr + extreme_idx), ascending)) {
        extreme_idx = current;
      }
    }

    if (extreme_idx != sorted_boundary) {
      swap(arr + sorted_boundary, arr + extreme_idx);
    }
  }
}

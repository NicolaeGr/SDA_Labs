#include "sort.h"

static inline void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

static inline int is_better_choice(int candidate, int current_best, int ascending) {
  return ascending ? candidate < current_best : candidate > current_best;
}

void selection_sort_array(int *arr, size_t size, int ascending) {
  if (size < 2) {
    return;
  }

  for (size_t sorted_boundary = 0; sorted_boundary < size - 1; sorted_boundary++) {
    size_t extreme_idx = sorted_boundary;

    for (size_t current = sorted_boundary + 1; current < size; current++) {
      if (is_better_choice(arr[current], arr[extreme_idx], ascending)) {
        extreme_idx = current;
      }
    }

    if (extreme_idx != sorted_boundary) {
      swap(&arr[sorted_boundary], &arr[extreme_idx]);
    }
  }
}

void selection_sort_pointer(int *arr, size_t size, int ascending) {
  if (size < 2) {
    return;
  }

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

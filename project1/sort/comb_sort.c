#include "sort.h"

#define SHRINK_FACTOR 13

static inline int should_swap(int left, int right, int ascending) { return ascending ? left > right : left < right; }

static inline size_t calculate_next_gap(size_t current_gap) {
  size_t new_gap = (current_gap * 10) / SHRINK_FACTOR;
  return new_gap < 1 ? 1 : new_gap;
}

void comb_sort_array(int *arr, size_t size, int ascending) {
  if (size < 2) {
    return;
  }

  size_t gap = size;
  int swapped = 1;

  while (gap > 1 || swapped) {
    gap = calculate_next_gap(gap);
    swapped = 0;

    for (size_t i = 0; i + gap < size; i++) {
      if (should_swap(arr[i], arr[i + gap], ascending)) {
        int temp = arr[i];
        arr[i] = arr[i + gap];
        arr[i + gap] = temp;
        swapped = 1;
      }
    }
  }
}

void comb_sort_pointer(int *arr, size_t size, int ascending) {
  if (size < 2) {
    return;
  }

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

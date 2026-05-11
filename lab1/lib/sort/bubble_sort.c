#include "sort.h"

static inline int should_swap(int left, int right, int ascending) { return ascending ? left > right : left < right; }

void bubble_sort_array(int *arr, size_t size, int ascending) {
  if (size < 2) {
    return;
  }

  for (size_t pass = 0; pass < size - 1; pass++) {
    for (size_t current = 0; current < size - pass - 1; current++) {
      if (should_swap(arr[current], arr[current + 1], ascending)) {
        int temp = arr[current];
        arr[current] = arr[current + 1];
        arr[current + 1] = temp;
      }
    }
  }
}

void bubble_sort_pointer(int *arr, size_t size, int ascending) {
  if (size < 2) {
    return;
  }

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

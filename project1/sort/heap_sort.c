#include "sort.h"
#include "utils.h"
#include <stdlib.h>

static inline void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

static inline int is_better_priority(int candidate, int current, int ascending) {
  return ascending ? candidate > current : candidate < current;
}

static void heapify(int *arr, size_t heap_size, size_t root_idx, int ascending) {
  size_t extreme_idx = root_idx;
  size_t left_child = 2 * root_idx + 1;
  size_t right_child = 2 * root_idx + 2;

  if (left_child < heap_size && is_better_priority(*(arr + left_child), *(arr + extreme_idx), ascending)) {
    extreme_idx = left_child;
  }

  if (right_child < heap_size && is_better_priority(*(arr + right_child), *(arr + extreme_idx), ascending)) {
    extreme_idx = right_child;
  }

  if (extreme_idx != root_idx) {
    swap(arr + root_idx, arr + extreme_idx);
    heapify(arr, heap_size, extreme_idx, ascending);
  }
}

int *heap_sort_by_value(const int *arr, size_t size, int ascending) {
  int *copy = duplicate_vec1(arr, size);
  if (!copy) {
    return NULL;
  }
  heap_sort_by_pointer(copy, size, ascending);
  return copy;
}

void heap_sort_by_pointer(int *arr, size_t size, int ascending) {
  for (int i = size / 2 - 1; i >= 0; i--) {
    heapify(arr, size, i, ascending);
  }

  for (int i = size - 1; i > 0; i--) {
    swap(arr, arr + i);
    heapify(arr, i, 0, ascending);
  }
}

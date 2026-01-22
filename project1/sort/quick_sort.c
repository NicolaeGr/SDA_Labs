#include "sort.h"
#include "utils.h"
#include <stdlib.h>

static inline void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

static inline int belongs_left_of_pivot(int value, int pivot, int ascending) {
  return ascending ? value < pivot : value > pivot;
}

static size_t partition(int *arr, size_t low, size_t high, int ascending) {
  int pivot_value = *(arr + high);
  size_t partition_idx = low;

  for (size_t current = low; current < high; current++) {
    if (belongs_left_of_pivot(*(arr + current), pivot_value, ascending)) {
      swap(arr + partition_idx, arr + current);
      partition_idx++;
    }
  }

  swap(arr + partition_idx, arr + high);
  return partition_idx;
}

static void quick_sort_helper(int *arr, size_t low, size_t high, int ascending) {
  if (low < high) {
    size_t pi = partition(arr, low, high, ascending);
    if (pi > 0) {
      quick_sort_helper(arr, low, pi - 1, ascending);
    }
    quick_sort_helper(arr, pi + 1, high, ascending);
  }
}

int *quick_sort_by_value(const int *arr, size_t size, int ascending) {
  int *copy = duplicate_vec1(arr, size);
  if (!copy) {
    return NULL;
  }
  quick_sort_by_pointer(copy, size, ascending);
  return copy;
}

void quick_sort_by_pointer(int *arr, size_t size, int ascending) {
  if (size > 1) {
    quick_sort_helper(arr, 0, size - 1, ascending);
  }
}

#include "sort.h"

static inline void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

static inline int belongs_left_of_pivot(int value, int pivot, int ascending) {
  return ascending ? value < pivot : value > pivot;
}

static size_t partition_array(int *arr, size_t low, size_t high, int ascending) {
  int pivot_value = arr[high];
  size_t partition_idx = low;

  for (size_t current = low; current < high; current++) {
    if (belongs_left_of_pivot(arr[current], pivot_value, ascending)) {
      swap(&arr[partition_idx], &arr[current]);
      partition_idx++;
    }
  }

  swap(&arr[partition_idx], &arr[high]);
  return partition_idx;
}

static size_t partition_pointer(int *arr, size_t low, size_t high, int ascending) {
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

static void quick_sort_helper_array(int *arr, size_t low, size_t high, int ascending) {
  if (low < high) {
    size_t pi = partition_array(arr, low, high, ascending);
    if (pi > 0) {
      quick_sort_helper_array(arr, low, pi - 1, ascending);
    }
    quick_sort_helper_array(arr, pi + 1, high, ascending);
  }
}

static void quick_sort_helper_pointer(int *arr, size_t low, size_t high, int ascending) {
  if (low < high) {
    size_t pi = partition_pointer(arr, low, high, ascending);
    if (pi > 0) {
      quick_sort_helper_pointer(arr, low, pi - 1, ascending);
    }
    quick_sort_helper_pointer(arr, pi + 1, high, ascending);
  }
}

void quick_sort_array(int *arr, size_t size, int ascending) {
  if (size > 1) {
    quick_sort_helper_array(arr, 0, size - 1, ascending);
  }
}

void quick_sort_pointer(int *arr, size_t size, int ascending) {
  if (size > 1) {
    quick_sort_helper_pointer(arr, 0, size - 1, ascending);
  }
}

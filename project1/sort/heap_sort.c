#include "sort.h"

static inline void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

static inline int is_better_priority(int candidate, int current, int ascending) {
  return ascending ? candidate > current : candidate < current;
}

static void heapify_array(int *arr, size_t heap_size, size_t root_idx, int ascending) {
  size_t extreme_idx = root_idx;
  size_t left_child = 2 * root_idx + 1;
  size_t right_child = 2 * root_idx + 2;

  if (left_child < heap_size && is_better_priority(arr[left_child], arr[extreme_idx], ascending)) {
    extreme_idx = left_child;
  }

  if (right_child < heap_size && is_better_priority(arr[right_child], arr[extreme_idx], ascending)) {
    extreme_idx = right_child;
  }

  if (extreme_idx != root_idx) {
    swap(&arr[root_idx], &arr[extreme_idx]);
    heapify_array(arr, heap_size, extreme_idx, ascending);
  }
}

static void heapify_pointer(int *arr, size_t heap_size, size_t root_idx, int ascending) {
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
    heapify_pointer(arr, heap_size, extreme_idx, ascending);
  }
}

void heap_sort_array(int *arr, size_t size, int ascending) {
  if (size < 2) {
    return;
  }

  for (int i = (int)size / 2 - 1; i >= 0; i--) {
    heapify_array(arr, size, (size_t)i, ascending);
  }

  for (int i = (int)size - 1; i > 0; i--) {
    swap(&arr[0], &arr[i]);
    heapify_array(arr, (size_t)i, 0, ascending);
  }
}

void heap_sort_pointer(int *arr, size_t size, int ascending) {
  if (size < 2) {
    return;
  }

  for (int i = (int)size / 2 - 1; i >= 0; i--) {
    heapify_pointer(arr, size, (size_t)i, ascending);
  }

  for (int i = (int)size - 1; i > 0; i--) {
    swap(arr, arr + i);
    heapify_pointer(arr, (size_t)i, 0, ascending);
  }
}

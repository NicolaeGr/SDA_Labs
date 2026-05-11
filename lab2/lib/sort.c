#include "sort.h"
#include <stdlib.h>
#include <string.h>

static inline void *get_element(void *base, size_t index, size_t elem_size) {
  return (char *)base + (index * elem_size);
}

static void swap_elements(void *a, void *b, size_t elem_size) {
  char temp[elem_size];
  memcpy(temp, a, elem_size);
  memcpy(a, b, elem_size);
  memcpy(b, temp, elem_size);
}

static int partition(void *base, int left, int right, size_t elem_size, int (*compare)(const void *, const void *)) {
  void *pivot = get_element(base, right, elem_size);
  int i = left - 1;

  for (int j = left; j < right; j++) {
    void *elem_j = get_element(base, j, elem_size);
    if (compare(elem_j, pivot) <= 0) {
      i++;
      void *elem_i = get_element(base, i, elem_size);
      swap_elements(elem_i, elem_j, elem_size);
    }
  }

  void *elem_i_plus_1 = get_element(base, i + 1, elem_size);
  swap_elements(elem_i_plus_1, pivot, elem_size);

  return i + 1;
}

static void quick_sort_recursive(void *base, int left, int right, size_t elem_size,
                                 int (*compare)(const void *, const void *)) {
  if (left < right) {
    int pivot_index = partition(base, left, right, elem_size, compare);
    quick_sort_recursive(base, left, pivot_index - 1, elem_size, compare);
    quick_sort_recursive(base, pivot_index + 1, right, elem_size, compare);
  }
}

void quick_sort(void *base, size_t count, size_t elem_size, int (*compare)(const void *, const void *)) {
  if (base == NULL || count <= 1 || elem_size == 0 || compare == NULL) {
    return;
  }
  quick_sort_recursive(base, 0, count - 1, elem_size, compare);
}

static void merge(void *base, int left, int mid, int right, void *temp, size_t elem_size,
                  int (*compare)(const void *, const void *)) {
  int i = left;
  int j = mid + 1;
  int k = left;

  while (i <= mid && j <= right) {
    void *elem_i = get_element(base, i, elem_size);
    void *elem_j = get_element(base, j, elem_size);
    void *temp_k = get_element(temp, k, elem_size);

    if (compare(elem_i, elem_j) <= 0) {
      memcpy(temp_k, elem_i, elem_size);
      i++;
    } else {
      memcpy(temp_k, elem_j, elem_size);
      j++;
    }
    k++;
  }

  while (i <= mid) {
    void *elem_i = get_element(base, i, elem_size);
    void *temp_k = get_element(temp, k, elem_size);
    memcpy(temp_k, elem_i, elem_size);
    i++;
    k++;
  }

  while (j <= right) {
    void *elem_j = get_element(base, j, elem_size);
    void *temp_k = get_element(temp, k, elem_size);
    memcpy(temp_k, elem_j, elem_size);
    j++;
    k++;
  }

  for (i = left; i <= right; i++) {
    void *elem_i = get_element(base, i, elem_size);
    void *temp_i = get_element(temp, i, elem_size);
    memcpy(elem_i, temp_i, elem_size);
  }
}

static void merge_sort_recursive(void *base, int left, int right, void *temp, size_t elem_size,
                                 int (*compare)(const void *, const void *)) {
  if (left < right) {
    int mid = left + (right - left) / 2;

    merge_sort_recursive(base, left, mid, temp, elem_size, compare);
    merge_sort_recursive(base, mid + 1, right, temp, elem_size, compare);
    merge(base, left, mid, right, temp, elem_size, compare);
  }
}

void merge_sort(void *base, size_t count, size_t elem_size, int (*compare)(const void *, const void *)) {
  if (base == NULL || count <= 1 || elem_size == 0 || compare == NULL) {
    return;
  }

  void *temp = malloc(count * elem_size);
  if (temp == NULL) {
    return;
  }

  merge_sort_recursive(base, 0, count - 1, temp, elem_size, compare);

  free(temp);
}

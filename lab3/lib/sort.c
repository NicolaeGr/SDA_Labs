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

static void heapify(void *base, size_t heap_size, size_t root_idx, size_t elem_size,
                    int (*compare)(const void *, const void *)) {
  size_t largest = root_idx;
  size_t left = 2 * root_idx + 1;
  size_t right = 2 * root_idx + 2;

  if (left < heap_size) {
    void *l = get_element(base, left, elem_size);
    void *largest_e = get_element(base, largest, elem_size);
    if (compare(l, largest_e) > 0) {
      largest = left;
    }
  }

  if (right < heap_size) {
    void *r = get_element(base, right, elem_size);
    void *largest_e = get_element(base, largest, elem_size);
    if (compare(r, largest_e) > 0) {
      largest = right;
    }
  }

  if (largest != root_idx) {
    void *root = get_element(base, root_idx, elem_size);
    void *largest_e = get_element(base, largest, elem_size);
    swap_elements(root, largest_e, elem_size);
    heapify(base, heap_size, largest, elem_size, compare);
  }
}

void heap_sort(void *base, size_t count, size_t elem_size, int (*compare)(const void *, const void *)) {
  if (!base || count < 2 || elem_size == 0 || compare == NULL) {
    return;
  }

  // Build heap (max-heap)
  for (size_t i = count / 2; i > 0; i--) {
    heapify(base, count, i - 1, elem_size, compare);
  }

  // One by one extract elements
  for (size_t i = count; i > 1; i--) {
    swap_elements(get_element(base, 0, elem_size), get_element(base, i - 1, elem_size), elem_size);
    heapify(base, i - 1, 0, elem_size, compare);
  }
}

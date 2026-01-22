#include "sort.h"
#include "utils.h"
#include <stdlib.h>

static inline int should_shift_gap(int previous, int key, int ascending) {
  return ascending ? previous > key : previous < key;
}

int *shell_sort_by_value(const int *arr, size_t size, int ascending) {
  int *copy = duplicate_vec1(arr, size);
  if (!copy) {
    return NULL;
  }
  shell_sort_by_pointer(copy, size, ascending);
  return copy;
}

void shell_sort_by_pointer(int *arr, size_t size, int ascending) {
  for (size_t gap = size / 2; gap > 0; gap /= 2) {
    for (size_t current = gap; current < size; current++) {
      int key_value = *(arr + current);
      size_t insert_pos = current;

      while (insert_pos >= gap && should_shift_gap(*(arr + insert_pos - gap), key_value, ascending)) {
        *(arr + insert_pos) = *(arr + insert_pos - gap);
        insert_pos -= gap;
      }

      *(arr + insert_pos) = key_value;
    }
  }
}

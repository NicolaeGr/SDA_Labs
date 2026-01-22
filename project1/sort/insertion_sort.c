#include "sort.h"
#include "utils.h"
#include <stdlib.h>

static inline int should_shift(int previous, int key, int ascending) {
  return ascending ? previous > key : previous < key;
}

int *insertion_sort_by_value(const int *arr, size_t size, int ascending) {
  int *copy = duplicate_vec1(arr, size);
  if (!copy) {
    return NULL;
  }
  insertion_sort_by_pointer(copy, size, ascending);
  return copy;
}

void insertion_sort_by_pointer(int *arr, size_t size, int ascending) {
  for (size_t unsorted_idx = 1; unsorted_idx < size; unsorted_idx++) {
    int key_value = *(arr + unsorted_idx);
    size_t insert_pos = unsorted_idx;

    while (insert_pos > 0 && should_shift(*(arr + insert_pos - 1), key_value, ascending)) {
      *(arr + insert_pos) = *(arr + insert_pos - 1);
      insert_pos--;
    }

    *(arr + insert_pos) = key_value;
  }
}

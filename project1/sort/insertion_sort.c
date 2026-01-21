#include "sort.h"
#include "utils.h"
#include <stdlib.h>

int *insertion_sort_by_value(const int *arr, size_t size, int ascending) {
  int *copy = duplicate_vec1(arr, size);
  if (!copy) {
    return NULL;
  }
  insertion_sort_by_pointer(copy, size, ascending);
  return copy;
}

void insertion_sort_by_pointer(int *arr, size_t size, int ascending) {
  (void)arr;
  (void)size;
  (void)ascending;
}

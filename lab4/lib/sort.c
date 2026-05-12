#include "sort.h"

#include <stdlib.h>
#include <string.h>

void insertion_sort(void *base, size_t count, size_t elem_size, int (*compare)(const void *, const void *)) {
  if (!base || count < 2 || elem_size == 0 || !compare) {
    return;
  }

  char *array = (char *)base;
  char *temp = malloc(elem_size);
  if (!temp) {
    return;
  }

  for (size_t i = 1; i < count; i++) {
    memcpy(temp, array + (i * elem_size), elem_size);
    size_t j = i;

    while (j > 0 && compare(array + ((j - 1) * elem_size), temp) > 0) {
      memcpy(array + (j * elem_size), array + ((j - 1) * elem_size), elem_size);
      j--;
    }

    memcpy(array + (j * elem_size), temp, elem_size);
  }

  free(temp);
}

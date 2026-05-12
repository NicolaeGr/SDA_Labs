#ifndef LAB4_SORT_H
#define LAB4_SORT_H

#include <stddef.h>

void insertion_sort(void *base, size_t count, size_t elem_size, int (*compare)(const void *, const void *));

#endif

#ifndef LAB3_SORT_H
#define LAB3_SORT_H

#include <stddef.h>

// Generic heap sort implementation (used by lab3)
void heap_sort(void *base, size_t count, size_t elem_size, int (*compare)(const void *, const void *));

#endif

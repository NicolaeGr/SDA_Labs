#ifndef LAB3_LINEAR_SEARCH_H
#define LAB3_LINEAR_SEARCH_H

#include <stddef.h>

typedef int (*search_cmp_fn)(const void *item, const void *search_key);

int linear_search(const void *items, size_t count, size_t item_size, search_cmp_fn cmp, const void *search_key);

#endif

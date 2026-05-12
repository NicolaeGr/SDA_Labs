#include "linear_search.h"
#include <string.h>

int linear_search(const void *items, size_t count, size_t item_size, search_cmp_fn cmp, const void *search_key) {
  if (!items || !cmp) {
    return -1;
  }

  for (size_t i = 0; i < count; i++) {
    const char *base = (const char *)items;
    if (cmp(base + i * item_size, search_key) == 0) {
      return (int)i;
    }
  }

  return -1;
}

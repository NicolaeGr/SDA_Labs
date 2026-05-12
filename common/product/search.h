#ifndef COMMON_PRODUCT_SEARCH_H
#define COMMON_PRODUCT_SEARCH_H

#include "product.h"

int product_search_by_field(const Product *items, size_t count, ProductSortField field, const char *query);

#endif // COMMON_PRODUCT_SEARCH_H

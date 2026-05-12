#ifndef COMMON_PRODUCT_PRODUCT_H
#define COMMON_PRODUCT_PRODUCT_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  char name[100];
  char country[50];
  char manufacturer[100];
  char item[100];
  double price;
} Product;

typedef struct {
  Product *items;
  size_t count;
  size_t capacity;
} ProductArray;

typedef struct {
  int (*compare)(const void *a, const void *b);
  const char *name;
} ProductFieldInfo;

typedef enum {
  PRODUCT_SORT_BY_NAME = 0,
  PRODUCT_SORT_BY_COUNTRY = 1,
  PRODUCT_SORT_BY_MANUFACTURER = 2,
  PRODUCT_SORT_BY_ITEM = 3,
  PRODUCT_SORT_BY_PRICE = 4,
  PRODUCT_SORT_FIELD_COUNT = 5
} ProductSortField;

ProductArray *product_array_create(size_t initial_capacity);
void product_array_free(ProductArray *arr);
bool product_array_add(ProductArray *arr, const Product *item);
void product_array_clear(ProductArray *arr);
void product_array_reset(ProductArray *arr);

void product_input(Product *item);
void product_input_array(Product *items, size_t count);

void product_display(const Product *item);
void product_display_array(const Product *items, size_t count, const char *title);

size_t product_create_mock_data(Product **items);

void product_set_sort_direction(int ascending);
int product_compare_by_name(const void *a, const void *b);
int product_compare_by_country(const void *a, const void *b);
int product_compare_by_manufacturer(const void *a, const void *b);
int product_compare_by_item(const void *a, const void *b);
int product_compare_by_price(const void *a, const void *b);

const ProductFieldInfo *product_get_sort_fields(void);

#endif // COMMON_PRODUCT_PRODUCT_H

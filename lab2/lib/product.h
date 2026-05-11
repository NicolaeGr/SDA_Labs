#ifndef PRODUCT_H
#define PRODUCT_H

#include <stddef.h>

typedef struct {
  char name[100];
  char country[50];
  char manufacturer[100];
  char item[100];
  double price;
} Product;

void product_input(Product *product);
void product_input_array(Product *products, size_t count);

void product_display(const Product *product);
void product_display_array(const Product *products, size_t count, const char *title);

size_t product_create_mock_data(Product **products);

void product_set_sort_direction(int ascending);
int product_compare_by_name(const void *a, const void *b);
int product_compare_by_country(const void *a, const void *b);
int product_compare_by_manufacturer(const void *a, const void *b);
int product_compare_by_item(const void *a, const void *b);
int product_compare_by_price(const void *a, const void *b);

#endif // PRODUCT_H

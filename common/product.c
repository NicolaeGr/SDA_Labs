#include "product.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/utils.h"

#define PRODUCT_INITIAL_CAPACITY 10
#define PRODUCT_GROWTH_FACTOR 1.5

static int read_line_into(char *buffer, size_t buffer_size) {
  if (!fgets(buffer, buffer_size, stdin)) {
    buffer[0] = '\0';
    return 0;
  }

  buffer[strcspn(buffer, "\n")] = 0;
  return 1;
}

ProductArray *product_array_create(size_t initial_capacity) {
  if (initial_capacity == 0) {
    initial_capacity = PRODUCT_INITIAL_CAPACITY;
  }

  ProductArray *arr = (ProductArray *)malloc(sizeof(ProductArray));
  if (!arr) {
    return NULL;
  }

  arr->items = (Product *)malloc(initial_capacity * sizeof(Product));
  if (!arr->items) {
    free(arr);
    return NULL;
  }

  arr->count = 0;
  arr->capacity = initial_capacity;
  return arr;
}

void product_array_free(ProductArray *arr) {
  if (!arr) {
    return;
  }

  free(arr->items);
  free(arr);
}

static bool product_array_grow(ProductArray *arr) {
  if (!arr || !arr->items) {
    return false;
  }

  size_t new_capacity = (size_t)(arr->capacity * PRODUCT_GROWTH_FACTOR);
  if (new_capacity <= arr->capacity) {
    new_capacity = arr->capacity + 10;
  }

  Product *new_items = (Product *)realloc(arr->items, new_capacity * sizeof(Product));
  if (!new_items) {
    return false;
  }

  arr->items = new_items;
  arr->capacity = new_capacity;
  return true;
}

bool product_array_add(ProductArray *arr, const Product *item) {
  if (!arr || !item) {
    return false;
  }

  if (arr->count >= arr->capacity && !product_array_grow(arr)) {
    return false;
  }

  memcpy(&arr->items[arr->count], item, sizeof(Product));
  arr->count++;
  return true;
}

void product_array_clear(ProductArray *arr) {
  if (!arr) {
    return;
  }

  arr->count = 0;
}

void product_array_reset(ProductArray *arr) {
  if (!arr) {
    return;
  }

  arr->count = 0;
  arr->capacity = PRODUCT_INITIAL_CAPACITY;

  Product *new_items = (Product *)realloc(arr->items, arr->capacity * sizeof(Product));
  if (new_items) {
    arr->items = new_items;
  }
}

void product_input(Product *item) {
  if (!item) {
    return;
  }

  printf("Enter product name: ");
  fflush(stdout);
  if (!read_line_into(item->name, sizeof(item->name))) {
    return;
  }

  printf("Enter country: ");
  fflush(stdout);
  if (!read_line_into(item->country, sizeof(item->country))) {
    return;
  }

  printf("Enter manufacturer: ");
  fflush(stdout);
  if (!read_line_into(item->manufacturer, sizeof(item->manufacturer))) {
    return;
  }

  printf("Enter item: ");
  fflush(stdout);
  if (!read_line_into(item->item, sizeof(item->item))) {
    return;
  }

  item->price = get_double_input_range("Enter price: ", 0.0, INFINITY);
}

void product_input_array(Product *items, size_t count) {
  if (!items) {
    return;
  }

  printf("\n=== Enter %zu Products ===\n", count);
  for (size_t i = 0; i < count; i++) {
    printf("\n--- Product %zu ---\n", i + 1);
    product_input(&items[i]);
  }
}

void product_display(const Product *item) {
  if (!item) {
    return;
  }

  printf("  %-20s | %-15s | %-20s | %-20s | $%.2f\n", item->name, item->country, item->manufacturer, item->item,
         item->price);
}

void product_display_array(const Product *items, size_t count, const char *title) {
  if (!items) {
    return;
  }

  if (title) {
    print_separator('=', 90);
    printf("%s\n", title);
  }

  print_separator('=', 90);
  printf("  %-20s | %-15s | %-20s | %-20s | %-10s\n", "Name", "Country", "Manufacturer", "Item", "Price");
  print_separator('-', 90);

  for (size_t i = 0; i < count; i++) {
    product_display(&items[i]);
  }

  print_separator('=', 90);
  printf("Total: %zu products\n", count);
}

size_t product_create_mock_data(Product **items) {
  const size_t count = 10;

  Product *data = (Product *)malloc(count * sizeof(Product));
  if (!data) {
    *items = NULL;
    return 0;
  }

  static const Product mock_data[] = {{"iPhone 15", "USA", "Apple", "Smartphone", 999.99},
                                      {"Galaxy S24", "South Korea", "Samsung", "Smartphone", 899.99},
                                      {"ThinkPad X1", "China", "Lenovo", "Laptop", 1299.99},
                                      {"MacBook Pro", "USA", "Apple", "Laptop", 2499.99},
                                      {"Pixel 8", "USA", "Google", "Smartphone", 699.99},
                                      {"Surface Laptop", "USA", "Microsoft", "Laptop", 1199.99},
                                      {"AirPods Pro", "USA", "Apple", "Headphones", 249.99},
                                      {"Xperia 5", "Japan", "Sony", "Smartphone", 799.99},
                                      {"ZenBook 14", "Taiwan", "ASUS", "Laptop", 899.99},
                                      {"OnePlus 12", "China", "OnePlus", "Smartphone", 749.99}};

  memcpy(data, mock_data, sizeof(mock_data));
  *items = data;
  return count;
}

static int sort_direction = 1;

void product_set_sort_direction(int ascending) { sort_direction = ascending ? 1 : -1; }

int product_compare_by_name(const void *a, const void *b) {
  const Product *item_a = (const Product *)a;
  const Product *item_b = (const Product *)b;
  return sort_direction * strcasecmp(item_a->name, item_b->name);
}

int product_compare_by_country(const void *a, const void *b) {
  const Product *item_a = (const Product *)a;
  const Product *item_b = (const Product *)b;
  return sort_direction * strcasecmp(item_a->country, item_b->country);
}

int product_compare_by_manufacturer(const void *a, const void *b) {
  const Product *item_a = (const Product *)a;
  const Product *item_b = (const Product *)b;
  return sort_direction * strcasecmp(item_a->manufacturer, item_b->manufacturer);
}

int product_compare_by_item(const void *a, const void *b) {
  const Product *item_a = (const Product *)a;
  const Product *item_b = (const Product *)b;
  return sort_direction * strcasecmp(item_a->item, item_b->item);
}

int product_compare_by_price(const void *a, const void *b) {
  const Product *item_a = (const Product *)a;
  const Product *item_b = (const Product *)b;

  int result;
  if (item_a->price < item_b->price) {
    result = -1;
  } else if (item_a->price > item_b->price) {
    result = 1;
  } else {
    result = 0;
  }

  return sort_direction * result;
}

static const ProductFieldInfo sort_fields[] = {{product_compare_by_name, "Name"},
                                               {product_compare_by_country, "Country"},
                                               {product_compare_by_manufacturer, "Manufacturer"},
                                               {product_compare_by_item, "Item"},
                                               {product_compare_by_price, "Price"}};

const ProductFieldInfo *product_get_sort_fields(void) { return sort_fields; }

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/utils.h"

#include "product.h"

static int read_line_into(char *buffer, size_t buffer_size) {
  if (!fgets(buffer, buffer_size, stdin)) {
    buffer[0] = '\0';
    return 0;
  }

  buffer[strcspn(buffer, "\n")] = 0;
  return 1;
}

void product_input(Product *product) {
  if (!product)
    return;

  printf("Enter product name: ");
  fflush(stdout);
  if (!read_line_into(product->name, sizeof(product->name))) {
    return;
  }

  printf("Enter country: ");
  fflush(stdout);
  if (!read_line_into(product->country, sizeof(product->country))) {
    return;
  }

  printf("Enter manufacturer: ");
  fflush(stdout);
  if (!read_line_into(product->manufacturer, sizeof(product->manufacturer))) {
    return;
  }

  printf("Enter item: ");
  fflush(stdout);
  if (!read_line_into(product->item, sizeof(product->item))) {
    return;
  }

  product->price = get_double_input_range("Enter price: ", 0.0, INFINITY);
}

void product_input_array(Product *products, size_t count) {
  if (!products)
    return;

  printf("\n=== Enter %zu Products ===\n", count);
  for (size_t i = 0; i < count; i++) {
    printf("\n--- Product %zu ---\n", i + 1);
    product_input(&products[i]);
  }
}

void product_display(const Product *product) {
  if (!product)
    return;

  printf("  %-20s | %-15s | %-20s | %-20s | $%.2f\n", product->name, product->country, product->manufacturer,
         product->item, product->price);
}

void product_display_array(const Product *products, size_t count, const char *title) {
  if (!products)
    return;

  if (title) {
    print_separator('=', 90);
    printf("%s\n", title);
  }
  print_separator('=', 90);
  printf("  %-20s | %-15s | %-20s | %-20s | %-10s\n", "Name", "Country", "Manufacturer", "Item", "Price");
  print_separator('-', 90);

  for (size_t i = 0; i < count; i++) {
    product_display(&products[i]);
  }

  print_separator('=', 90);
  printf("Total: %zu products\n", count);
}

size_t product_create_mock_data(Product **products) {
  const size_t count = 10;

  Product *data = (Product *)malloc(count * sizeof(Product));
  if (!data) {
    *products = NULL;
    return 0;
  }

  strcpy(data[0].name, "iPhone 15");
  strcpy(data[0].country, "USA");
  strcpy(data[0].manufacturer, "Apple");
  strcpy(data[0].item, "Smartphone");
  data[0].price = 999.99;

  strcpy(data[1].name, "Galaxy S24");
  strcpy(data[1].country, "South Korea");
  strcpy(data[1].manufacturer, "Samsung");
  strcpy(data[1].item, "Smartphone");
  data[1].price = 899.99;

  strcpy(data[2].name, "ThinkPad X1");
  strcpy(data[2].country, "China");
  strcpy(data[2].manufacturer, "Lenovo");
  strcpy(data[2].item, "Laptop");
  data[2].price = 1299.99;

  strcpy(data[3].name, "MacBook Pro");
  strcpy(data[3].country, "USA");
  strcpy(data[3].manufacturer, "Apple");
  strcpy(data[3].item, "Laptop");
  data[3].price = 2499.99;

  strcpy(data[4].name, "Pixel 8");
  strcpy(data[4].country, "USA");
  strcpy(data[4].manufacturer, "Google");
  strcpy(data[4].item, "Smartphone");
  data[4].price = 699.99;

  strcpy(data[5].name, "Surface Laptop");
  strcpy(data[5].country, "USA");
  strcpy(data[5].manufacturer, "Microsoft");
  strcpy(data[5].item, "Laptop");
  data[5].price = 1199.99;

  strcpy(data[6].name, "AirPods Pro");
  strcpy(data[6].country, "USA");
  strcpy(data[6].manufacturer, "Apple");
  strcpy(data[6].item, "Headphones");
  data[6].price = 249.99;

  strcpy(data[7].name, "Xperia 5");
  strcpy(data[7].country, "Japan");
  strcpy(data[7].manufacturer, "Sony");
  strcpy(data[7].item, "Smartphone");
  data[7].price = 799.99;

  strcpy(data[8].name, "ZenBook 14");
  strcpy(data[8].country, "Taiwan");
  strcpy(data[8].manufacturer, "ASUS");
  strcpy(data[8].item, "Laptop");
  data[8].price = 899.99;

  strcpy(data[9].name, "OnePlus 12");
  strcpy(data[9].country, "China");
  strcpy(data[9].manufacturer, "OnePlus");
  strcpy(data[9].item, "Smartphone");
  data[9].price = 749.99;

  *products = data;
  return count;
}

static int sort_direction = 1; // 1 for ascending, -1 for descending

void product_set_sort_direction(int ascending) { sort_direction = ascending ? 1 : -1; }

int product_compare_by_name(const void *a, const void *b) {
  const Product *prod_a = (const Product *)a;
  const Product *prod_b = (const Product *)b;
  return sort_direction * strcasecmp(prod_a->name, prod_b->name);
}

int product_compare_by_country(const void *a, const void *b) {
  const Product *prod_a = (const Product *)a;
  const Product *prod_b = (const Product *)b;
  return sort_direction * strcasecmp(prod_a->country, prod_b->country);
}

int product_compare_by_manufacturer(const void *a, const void *b) {
  const Product *prod_a = (const Product *)a;
  const Product *prod_b = (const Product *)b;
  return sort_direction * strcasecmp(prod_a->manufacturer, prod_b->manufacturer);
}

int product_compare_by_item(const void *a, const void *b) {
  const Product *prod_a = (const Product *)a;
  const Product *prod_b = (const Product *)b;
  return sort_direction * strcasecmp(prod_a->item, prod_b->item);
}

int product_compare_by_price(const void *a, const void *b) {
  const Product *prod_a = (const Product *)a;
  const Product *prod_b = (const Product *)b;

  int result;
  if (prod_a->price < prod_b->price)
    result = -1;
  else if (prod_a->price > prod_b->price)
    result = 1;
  else
    result = 0;

  return sort_direction * result;
}

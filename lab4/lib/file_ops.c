#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/product/search.h"
#include "common/utils.h"

#include "file_ops.h"
#include "sort.h"

static bool write_product_line(FILE *file, const Product *product) {
  return fprintf(file, "%s|%s|%s|%s|%.2f\n", product->name, product->country, product->manufacturer, product->item,
                 product->price) >= 0;
}

static bool parse_product_line(const char *line, Product *product) {
  if (!line || !product) {
    return false;
  }

  Product parsed = {0};
  if (sscanf(line, "%99[^|]|%49[^|]|%99[^|]|%99[^|]|%lf", parsed.name, parsed.country, parsed.manufacturer, parsed.item,
             &parsed.price) != 5) {
    return false;
  }

  *product = parsed;
  return true;
}

static bool load_products_internal(const char *filename, ProductArray *products) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Failed to open file");
    return false;
  }

  char line[512];
  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';

    if (line[0] == '\0') {
      continue;
    }

    Product product = {0};
    if (!parse_product_line(line, &product)) {
      fprintf(stderr, "Error: malformed text line in file\n");
      fclose(file);
      return false;
    }

    if (!product_array_add(products, &product)) {
      fprintf(stderr, "Error: failed to grow product array\n");
      fclose(file);
      return false;
    }
  }

  fclose(file);
  return true;
}

bool text_file_save_all(const char *filename, const ProductArray *products) {
  if (!filename || !products) {
    return false;
  }

  FILE *file = fopen(filename, "w");
  if (!file) {
    perror("Failed to create file");
    return false;
  }

  for (size_t i = 0; i < products->count; i++) {
    if (!write_product_line(file, &products->items[i])) {
      perror("Failed to write product");
      fclose(file);
      return false;
    }
  }

  fclose(file);
  return true;
}

bool text_file_load_all(const char *filename, ProductArray **products) {
  if (!filename || !products || !*products) {
    return false;
  }

  return load_products_internal(filename, *products);
}

bool text_file_create_and_populate(const char *filename, ProductArray **out_products) {
  if (!filename || !out_products) {
    return false;
  }

  ProductArray *products = product_array_create(10);
  if (!products) {
    return false;
  }

  int count = get_int_input_range("How many products do you want to enter? ", 0, 100);
  for (int i = 0; i < count; i++) {
    Product product = {0};
    printf("\n--- Product %d of %d ---\n", i + 1, count);
    product_input(&product);

    if (!product_array_add(products, &product)) {
      fprintf(stderr, "Error: failed to add product\n");
      product_array_free(products);
      return false;
    }
  }

  if (!text_file_save_all(filename, products)) {
    product_array_free(products);
    return false;
  }

  *out_products = products;
  printf("\nFile '%s' created with %zu products\n", filename, products->count);
  return true;
}

bool text_file_read_and_display(const char *filename) {
  if (!filename) {
    return false;
  }

  ProductArray *products = product_array_create(10);
  if (!products) {
    return false;
  }

  if (!load_products_internal(filename, products)) {
    product_array_free(products);
    return false;
  }

  if (products->count == 0) {
    printf("\nFile is empty.\n");
  } else {
    printf("\n");
    product_display_array(products->items, products->count, "Text File Contents");
  }

  product_array_free(products);
  return true;
}

bool text_file_append_product(const char *filename, const Product *product) {
  if (!filename || !product) {
    return false;
  }

  FILE *file = fopen(filename, "a");
  if (!file) {
    perror("Failed to open file");
    return false;
  }

  bool ok = write_product_line(file, product);
  fclose(file);

  if (!ok) {
    perror("Failed to append product");
    return false;
  }

  printf("Product appended successfully\n");
  return true;
}

bool text_file_update_product(const char *filename, size_t index, const Product *product) {
  if (!filename || !product) {
    return false;
  }

  ProductArray *products = product_array_create(10);
  if (!products) {
    return false;
  }

  if (!load_products_internal(filename, products)) {
    product_array_free(products);
    return false;
  }

  if (index >= products->count) {
    printf("Invalid index\n");
    product_array_free(products);
    return false;
  }

  products->items[index] = *product;

  bool ok = text_file_save_all(filename, products);
  product_array_free(products);

  if (ok) {
    printf("Product updated successfully\n");
  }
  return ok;
}

int text_file_search_by_field(const char *filename, int field_index, const char *search_value) {
  if (!filename || !search_value) {
    return -1;
  }

  ProductArray *products = product_array_create(10);
  if (!products) {
    return -1;
  }

  if (!load_products_internal(filename, products)) {
    product_array_free(products);
    return -1;
  }

  int found = product_search_by_field(products->items, products->count, (ProductSortField)field_index, search_value);
  product_array_free(products);
  return found;
}

bool text_file_sort_by_field(const char *filename, int field_index, int sort_direction) {
  if (!filename) {
    return false;
  }

  ProductArray *products = product_array_create(10);
  if (!products) {
    return false;
  }

  if (!load_products_internal(filename, products)) {
    product_array_free(products);
    return false;
  }

  if (products->count == 0) {
    printf("File is empty, nothing to sort\n");
    product_array_free(products);
    return true;
  }

  const ProductFieldInfo *fields = product_get_sort_fields();
  if (field_index < 0 || field_index >= PRODUCT_SORT_FIELD_COUNT) {
    product_array_free(products);
    return false;
  }

  product_set_sort_direction(sort_direction);
  insertion_sort(products->items, products->count, sizeof(Product), fields[field_index].compare);

  bool ok = text_file_save_all(filename, products);
  product_array_free(products);

  if (ok) {
    printf("File sorted successfully\n");
  }
  return ok;
}

bool text_file_delete_product(const char *filename, size_t index) {
  if (!filename) {
    return false;
  }

  ProductArray *products = product_array_create(10);
  if (!products) {
    return false;
  }

  if (!load_products_internal(filename, products)) {
    product_array_free(products);
    return false;
  }

  if (index >= products->count) {
    printf("Invalid index\n");
    product_array_free(products);
    return false;
  }

  for (size_t i = index; i + 1 < products->count; i++) {
    products->items[i] = products->items[i + 1];
  }
  products->count--;

  bool ok = text_file_save_all(filename, products);
  product_array_free(products);

  if (ok) {
    printf("Product deleted successfully\n");
  }
  return ok;
}

bool text_file_delete_file(const char *filename) {
  if (!filename) {
    return false;
  }

  if (remove(filename) != 0) {
    perror("Failed to delete file");
    return false;
  }

  printf("File '%s' deleted successfully\n", filename);
  return true;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/product/search.h"
#include "common/utils.h"

#include "file_ops.h"
#include "sort.h"

bool file_create_and_populate(const char *filename, ProductArray **out_products) {
  if (!filename || !out_products) {
    return false;
  }

  FILE *file = fopen(filename, "wb");
  if (!file) {
    perror("Failed to create file");
    return false;
  }

  ProductArray *products = product_array_create(10);
  if (!products) {
    fclose(file);
    return false;
  }

  int product_count = get_int_input_range("How many products do you want to enter? ", 0, 100);

  for (int i = 0; i < product_count; i++) {
    Product product = {0};
    printf("\n--- Product %d of %d ---\n", i + 1, product_count);
    product_input(&product);

    if (!product_array_add(products, &product)) {
      printf("Failed to add product to array\n");
      break;
    }
  }

  if (products->count > 0) {
    size_t written = fwrite(products->items, sizeof(Product), products->count, file);
    if (written != products->count) {
      perror("Failed to write products to file");
      fclose(file);
      product_array_free(products);
      return false;
    }
  }

  fclose(file);
  *out_products = products;
  printf("\nFile '%s' created with %zu products\n", filename, products->count);
  return true;
}

bool file_read_and_display(const char *filename) {
  if (!filename) {
    return false;
  }

  FILE *file = fopen(filename, "rb");
  if (!file) {
    perror("Failed to open file");
    return false;
  }

  ProductArray *products = product_array_create(10);
  if (!products) {
    fclose(file);
    return false;
  }

  Product product;
  while (fread(&product, sizeof(Product), 1, file) == 1) {
    product_array_add(products, &product);
  }

  if (ferror(file)) {
    perror("Error reading file");
    fclose(file);
    product_array_free(products);
    return false;
  }

  fclose(file);

  if (products->count == 0) {
    printf("\nFile is empty.\n");
  } else {
    printf("\n");
    product_display_array(products->items, products->count, "File Contents");
  }

  product_array_free(products);
  return true;
}

bool file_append_product(const char *filename, const Product *product) {
  if (!filename || !product) {
    return false;
  }

  FILE *file = fopen(filename, "ab");
  if (!file) {
    perror("Failed to open file");
    return false;
  }

  if (fwrite(product, sizeof(Product), 1, file) != 1) {
    perror("Failed to append product");
    fclose(file);
    return false;
  }

  fclose(file);
  printf("Product appended successfully\n");
  return true;
}

bool file_update_product(const char *filename, size_t index, const Product *product) {
  if (!filename || !product) {
    return false;
  }

  FILE *file = fopen(filename, "r+b");
  if (!file) {
    perror("Failed to open file");
    return false;
  }

  if (fseek(file, (long)index * sizeof(Product), SEEK_SET) != 0) {
    perror("Failed to seek in file");
    fclose(file);
    return false;
  }

  if (fwrite(product, sizeof(Product), 1, file) != 1) {
    perror("Failed to update product");
    fclose(file);
    return false;
  }

  fclose(file);
  printf("Product updated successfully\n");
  return true;
}

int file_search_by_field(const char *filename, int field_index, const char *search_value) {
  if (!filename || !search_value) {
    return -1;
  }

  ProductArray *products = product_array_create(10);
  if (!products) {
    return -1;
  }

  FILE *file = fopen(filename, "rb");
  if (!file) {
    perror("Failed to open file");
    product_array_free(products);
    return -1;
  }

  Product product;
  while (fread(&product, sizeof(Product), 1, file) == 1) {
    product_array_add(products, &product);
  }

  fclose(file);

  if (field_index < 0 || field_index >= PRODUCT_SORT_FIELD_COUNT) {
    product_array_free(products);
    return -1;
  }

  int result = product_search_by_field(products->items, products->count, (ProductSortField)field_index, search_value);
  product_array_free(products);
  return result;
}

bool file_sort_by_field(const char *filename, int field_index, int sort_direction) {
  if (!filename) {
    return false;
  }

  ProductArray *products = product_array_create(10);
  if (!products) {
    return false;
  }

  if (!file_load_all(filename, &products)) {
    product_array_free(products);
    return false;
  }

  if (products->count == 0) {
    printf("File is empty, nothing to sort\n");
    product_array_free(products);
    return true;
  }

  const ProductFieldInfo *fields = product_get_sort_fields();
  product_set_sort_direction(sort_direction);

  heap_sort(products->items, products->count, sizeof(Product), fields[field_index].compare);

  bool success = file_save_all(filename, products);

  product_array_free(products);
  if (success) {
    printf("File sorted successfully\n");
  }
  return success;
}

bool file_delete_product(const char *filename, size_t index) {
  if (!filename) {
    return false;
  }

  ProductArray *products = product_array_create(10);
  if (!products) {
    return false;
  }

  if (!file_load_all(filename, &products)) {
    product_array_free(products);
    return false;
  }

  if (index >= products->count) {
    printf("Invalid index\n");
    product_array_free(products);
    return false;
  }

  for (size_t i = index; i < products->count - 1; i++) {
    products->items[i] = products->items[i + 1];
  }
  products->count--;

  bool success = file_save_all(filename, products);

  product_array_free(products);
  if (success) {
    printf("Product deleted successfully\n");
  }
  return success;
}

bool file_delete_file(const char *filename) {
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

bool file_load_all(const char *filename, ProductArray **products) {
  if (!filename || !products) {
    return false;
  }

  FILE *file = fopen(filename, "rb");
  if (!file) {
    perror("Failed to open file");
    return false;
  }

  Product product;
  while (fread(&product, sizeof(Product), 1, file) == 1) {
    if (!product_array_add(*products, &product)) {
      perror("Failed to add product to array");
      fclose(file);
      return false;
    }
  }

  if (ferror(file)) {
    perror("Error reading file");
    fclose(file);
    return false;
  }

  fclose(file);
  return true;
}

bool file_save_all(const char *filename, const ProductArray *products) {
  if (!filename || !products) {
    return false;
  }

  FILE *file = fopen(filename, "wb");
  if (!file) {
    perror("Failed to create file");
    return false;
  }

  if (products->count > 0) {
    if (fwrite(products->items, sizeof(Product), products->count, file) != products->count) {
      perror("Failed to write products to file");
      fclose(file);
      return false;
    }
  }

  fclose(file);
  return true;
}

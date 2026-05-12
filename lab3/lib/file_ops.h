#ifndef LAB3_FILE_OPS_H
#define LAB3_FILE_OPS_H

#include <stdbool.h>

#include "common/product/product.h"

// File operations for binary file I/O with Product structures

// 1. Create file, input products from keyboard, write to binary file
bool file_create_and_populate(const char *filename, ProductArray **out_products);

// 2. Read and display all products from binary file
bool file_read_and_display(const char *filename);

// 3. Append a new product to end of file
bool file_append_product(const char *filename, const Product *product);

// 4. Modify an existing product at given index
bool file_update_product(const char *filename, size_t index, const Product *product);

// 5. Search for products by field in file
int file_search_by_field(const char *filename, int field_index, const char *search_value);

// 6. Sort products in file by field (reads all, sorts, writes back)
bool file_sort_by_field(const char *filename, int field_index, int sort_direction);

// 7. Delete product at index from file
bool file_delete_product(const char *filename, size_t index);

// 8. Delete file from disk
bool file_delete_file(const char *filename);

// Helper: Load all products from file into ProductArray
bool file_load_all(const char *filename, ProductArray **products);

// Helper: Save ProductArray to file (overwrites)
bool file_save_all(const char *filename, const ProductArray *products);

#endif

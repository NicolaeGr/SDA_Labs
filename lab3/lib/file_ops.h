#ifndef LAB3_FILE_OPS_H
#define LAB3_FILE_OPS_H

#include <stdbool.h>

#include "common/product/product.h"

bool file_create_and_populate(const char *filename, ProductArray **out_products);
bool file_read_and_display(const char *filename);
bool file_append_product(const char *filename, const Product *product);
bool file_update_product(const char *filename, size_t index, const Product *product);
int file_search_by_field(const char *filename, int field_index, const char *search_value);
bool file_sort_by_field(const char *filename, int field_index, int sort_direction);
bool file_delete_product(const char *filename, size_t index);
bool file_delete_file(const char *filename);
bool file_load_all(const char *filename, ProductArray **products);
bool file_save_all(const char *filename, const ProductArray *products);

#endif

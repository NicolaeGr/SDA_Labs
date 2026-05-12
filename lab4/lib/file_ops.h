#ifndef LAB4_FILE_OPS_H
#define LAB4_FILE_OPS_H

#include <stdbool.h>
#include <stddef.h>

#include "common/product/product.h"

bool text_file_create_and_populate(const char *filename, ProductArray **out_products);
bool text_file_read_and_display(const char *filename);
bool text_file_append_product(const char *filename, const Product *product);
bool text_file_update_product(const char *filename, size_t index, const Product *product);
int text_file_search_by_field(const char *filename, int field_index, const char *search_value);
bool text_file_sort_by_field(const char *filename, int field_index, int sort_direction);
bool text_file_delete_product(const char *filename, size_t index);
bool text_file_delete_file(const char *filename);

bool text_file_load_all(const char *filename, ProductArray **products);
bool text_file_save_all(const char *filename, const ProductArray *products);

#endif

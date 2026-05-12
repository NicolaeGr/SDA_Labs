#ifndef LAB6_LIST_H
#define LAB6_LIST_H

#include <stdbool.h>
#include <stddef.h>

#include "common/product/product.h"

typedef struct ListNode {
  Product data;
  struct ListNode *next;
} ListNode;

typedef struct {
  ListNode *head;
  size_t size;
} ProductList;

ProductList *product_list_create(void);
void product_list_free(ProductList *list);
bool product_list_push_back(ProductList *list, const Product *item);
size_t product_list_length(const ProductList *list);
ListNode *product_list_node_at(ProductList *list, size_t index);
const ListNode *product_list_last(const ProductList *list);
bool product_list_swap(ProductList *list, size_t first_index, size_t second_index);
bool product_list_sort(ProductList *list, ProductSortField field, int ascending);
void product_list_display(const ProductList *list, const char *title);
bool product_list_to_array(const ProductList *list, Product **items, size_t *count);

#endif

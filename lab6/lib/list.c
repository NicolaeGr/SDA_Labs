#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/utils.h"

static ListNode *node_at(ProductList *list, size_t index) {
  if (!list || index >= list->size) {
    return NULL;
  }

  ListNode *current = list->head;
  for (size_t i = 0; i < index; i++) {
    current = current->next;
  }

  return current;
}

ProductList *product_list_create(void) {
  ProductList *list = (ProductList *)malloc(sizeof(ProductList));
  if (!list) {
    return NULL;
  }

  list->head = NULL;
  list->size = 0;
  return list;
}

void product_list_free(ProductList *list) {
  if (!list) {
    return;
  }

  ListNode *current = list->head;
  while (current) {
    ListNode *next = current->next;
    free(current);
    current = next;
  }

  list->head = NULL;
  list->size = 0;
  free(list);
}

bool product_list_push_back(ProductList *list, const Product *item) {
  if (!list || !item) {
    return false;
  }

  ListNode *node = (ListNode *)malloc(sizeof(ListNode));
  if (!node) {
    return false;
  }

  node->data = *item;
  node->next = NULL;

  if (!list->head) {
    list->head = node;
  } else {
    ListNode *tail = list->head;
    while (tail->next) {
      tail = tail->next;
    }
    tail->next = node;
  }

  list->size++;
  return true;
}

size_t product_list_length(const ProductList *list) { return list ? list->size : 0; }

ListNode *product_list_node_at(ProductList *list, size_t index) { return node_at(list, index); }

const ListNode *product_list_last(const ProductList *list) {
  if (!list || !list->head) {
    return NULL;
  }

  const ListNode *current = list->head;
  while (current->next) {
    current = current->next;
  }

  return current;
}

bool product_list_swap(ProductList *list, size_t first_index, size_t second_index) {
  if (!list || first_index >= list->size || second_index >= list->size) {
    return false;
  }

  if (first_index == second_index) {
    return true;
  }

  ListNode *first = node_at(list, first_index);
  ListNode *second = node_at(list, second_index);
  if (!first || !second) {
    return false;
  }

  Product temp = first->data;
  first->data = second->data;
  second->data = temp;
  return true;
}

bool product_list_sort(ProductList *list, ProductSortField field, int ascending) {
  if (!list || list->size < 2) {
    return true;
  }

  const ProductFieldInfo *fields = product_get_sort_fields();
  if (field < 0 || field >= PRODUCT_SORT_FIELD_COUNT) {
    return false;
  }

  product_set_sort_direction(ascending);
  int (*compare)(const void *, const void *) = fields[field].compare;

  for (ListNode *i = list->head; i != NULL; i = i->next) {
    ListNode *best = i;
    for (ListNode *j = i->next; j != NULL; j = j->next) {
      if (compare(&j->data, &best->data) < 0) {
        best = j;
      }
    }

    if (best != i) {
      Product temp = i->data;
      i->data = best->data;
      best->data = temp;
    }
  }

  return true;
}

void product_list_display(const ProductList *list, const char *title) {
  if (!list) {
    return;
  }

  if (title) {
    print_separator('=', 90);
    printf("%s\n", title);
  }

  print_separator('=', 90);
  printf("  %-5s | %-20s | %-15s | %-20s | %-20s | %-10s\n", "No", "Name", "Country", "Manufacturer", "Item", "Price");
  print_separator('-', 90);

  const ListNode *current = list->head;
  size_t index = 1;
  while (current) {
    printf("  %-5zu | %-20s | %-15s | %-20s | %-20s | %-10.2f\n", index, current->data.name, current->data.country,
           current->data.manufacturer, current->data.item, current->data.price);
    current = current->next;
    index++;
  }

  print_separator('=', 90);
  printf("Total: %zu elements\n", list->size);
}

bool product_list_to_array(const ProductList *list, Product **items, size_t *count) {
  if (!list || !items || !count) {
    return false;
  }

  *items = NULL;
  *count = 0;

  if (list->size == 0) {
    return true;
  }

  Product *array = (Product *)malloc(list->size * sizeof(Product));
  if (!array) {
    return false;
  }

  size_t index = 0;
  for (const ListNode *current = list->head; current != NULL; current = current->next) {
    array[index++] = current->data;
  }

  *items = array;
  *count = list->size;
  return true;
}

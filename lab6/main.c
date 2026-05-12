#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/menu.h"
#include "common/product/product.h"
#include "common/product/search.h"
#include "common/utils.h"

#include "lib/list.h"

static ProductList *product_list = NULL;

static void action_create_list(void);
static void action_input_list(void);
static void action_display_list(void);
static void action_search_list(void);
static void action_modify_element(void);
static void action_last_element_address(void);
static void action_list_length(void);
static void action_swap_elements(void);
static void action_sort_list(void);
static void action_free_list(void);

int main(void) {
  enter_alternate_screen();
  setup_terminal_cleanup();

  MenuItem items[] = {{1, "Create list in dynamic memory", action_create_list},
                      {2, "Input list elements from keyboard", action_input_list},
                      {3, "Display list elements", action_display_list},
                      {4, "Search element in list", action_search_list},
                      {5, "Modify fields of list element", action_modify_element},
                      {6, "Get address of last list element", action_last_element_address},
                      {7, "Get list length", action_list_length},
                      {8, "Swap two list elements", action_swap_elements},
                      {9, "Sort list (Selection Sort)", action_sort_list},
                      {10, "Free list memory", action_free_list},
                      {0, "Exit", NULL}};

  int item_count = sizeof(items) / sizeof(MenuItem);
  Menu *menu = menu_create("LAB 6 - Singly Linked List", items, item_count);
  if (!menu) {
    fprintf(stderr, "Failed to create menu\n");
    return 1;
  }

  menu_run(menu, 0);
  menu_free(menu);
  product_list_free(product_list);
  return 0;
}

static void print_field_menu(void) {
  const ProductFieldInfo *fields = product_get_sort_fields();
  for (int i = 0; i < PRODUCT_SORT_FIELD_COUNT; i++) {
    printf("%d. %s\n", i + 1, fields[i].name);
  }
}

static void action_create_list(void) {
  if (product_list) {
    product_list_free(product_list);
  }

  product_list = product_list_create();
  if (!product_list) {
    printf("Failed to create list.\n");
    return;
  }

  printf("Dynamic singly linked list created.\n");
}

static void action_input_list(void) {
  if (!product_list) {
    printf("Create the list first (option 1).\n");
    return;
  }

  int count = get_int_input_range("How many elements do you want to enter? ", 1, 1000);
  for (int i = 0; i < count; i++) {
    Product item = {0};
    printf("\n--- Element %d of %d ---\n", i + 1, count);
    product_input(&item);
    if (!product_list_push_back(product_list, &item)) {
      printf("Failed to add element to list.\n");
      return;
    }
  }

  printf("Inserted %d elements.\n", count);
}

static void action_display_list(void) {
  if (!product_list || product_list_length(product_list) == 0) {
    printf("List is empty.\n");
    return;
  }

  product_list_display(product_list, "List Contents");
}

static void action_search_list(void) {
  if (!product_list || product_list_length(product_list) == 0) {
    printf("List is empty.\n");
    return;
  }

  printf("\nSearch by field:\n");
  print_field_menu();
  int field_index = get_int_input_range("Select field: ", 1, PRODUCT_SORT_FIELD_COUNT) - 1;

  char query[128] = {0};
  printf("Enter search value: ");
  if (!fgets(query, sizeof(query), stdin)) {
    printf("Failed to read search value.\n");
    return;
  }
  query[strcspn(query, "\n")] = '\0';

  Product *array = NULL;
  size_t count = 0;
  if (!product_list_to_array(product_list, &array, &count)) {
    printf("Failed to prepare search data.\n");
    return;
  }

  (void)product_search_by_field(array, count, (ProductSortField)field_index, query);
  free(array);
}

static void action_modify_element(void) {
  if (!product_list || product_list_length(product_list) == 0) {
    printf("List is empty.\n");
    return;
  }

  int index = get_int_input_range("Select element number to modify: ", 1, (int)product_list_length(product_list));
  ListNode *node = product_list_node_at(product_list, (size_t)(index - 1));
  if (!node) {
    printf("Invalid element.\n");
    return;
  }

  product_input(&node->data);
  printf("Element updated successfully.\n");
}

static void action_last_element_address(void) {
  if (!product_list || product_list_length(product_list) == 0) {
    printf("List is empty.\n");
    return;
  }

  const ListNode *last = product_list_last(product_list);
  printf("Last element address: %p\n", (void *)last);
}

static void action_list_length(void) {
  if (!product_list) {
    printf("List is not created yet.\n");
    return;
  }

  printf("List length: %zu\n", product_list_length(product_list));
}

static void action_swap_elements(void) {
  if (!product_list || product_list_length(product_list) < 2) {
    printf("Need at least two elements to swap.\n");
    return;
  }

  int first = get_int_input_range("First element number: ", 1, (int)product_list_length(product_list));
  int second = get_int_input_range("Second element number: ", 1, (int)product_list_length(product_list));

  if (!product_list_swap(product_list, (size_t)(first - 1), (size_t)(second - 1))) {
    printf("Swap failed.\n");
    return;
  }

  printf("Elements swapped successfully.\n");
}

static void action_sort_list(void) {
  if (!product_list || product_list_length(product_list) < 2) {
    printf("Need at least two elements to sort.\n");
    return;
  }

  printf("\nSort by field:\n");
  print_field_menu();
  int field_index = get_int_input_range("Select field: ", 1, PRODUCT_SORT_FIELD_COUNT) - 1;

  printf("\nSort direction:\n");
  printf("1. Ascending\n");
  printf("2. Descending\n");
  int direction = get_int_input_range("Select direction: ", 1, 2);

  if (!product_list_sort(product_list, (ProductSortField)field_index, direction == 1)) {
    printf("Sort failed.\n");
    return;
  }

  printf("List sorted successfully.\n");
}

static void action_free_list(void) {
  if (!product_list) {
    printf("List is not created yet.\n");
    return;
  }

  product_list_free(product_list);
  product_list = NULL;
  printf("List memory released.\n");
}

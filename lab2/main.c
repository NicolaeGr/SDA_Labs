#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/menu.h"
#include "common/sort.h"
#include "common/utils.h"

#include "lib/product.h"

static Product *products = NULL;
static size_t product_count = 0;

void action_input_products(void);
void action_display_products(void);
void action_sort_products(void);
void action_load_mock_data(void);

int main(void) {
  enter_alternate_screen();
  setup_terminal_cleanup();

  MenuItem items[] = {{1, "Input products from keyboard", action_input_products},
                      {2, "Display products", action_display_products},
                      {3, "Sort products (Quick Sort / Merge Sort)", action_sort_products},
                      {4, "Load mock data (for testing)", action_load_mock_data},
                      {0, "Exit", NULL}};

  int item_count = sizeof(items) / sizeof(MenuItem);

  Menu *main_menu = menu_create("LAB 2 - Product Management System", items, item_count);

  if (!main_menu) {
    fprintf(stderr, "Error: Failed to create menu\n");
    return 1;
  }

  menu_run(main_menu, 0);

  menu_free(main_menu);

  if (products) {
    free(products);
  }

  return 0;
}

void action_input_products(void) {
  clear_screen();
  print_separator('=', 60);
  printf("Input Products from Keyboard\n");
  print_separator('=', 60);
  printf("\n");

  int count = get_int_input_range("How many products do you want to enter? ", 1, 100);

  if (products) {
    free(products);
  }

  products = (Product *)malloc(count * sizeof(Product));
  if (!products) {
    fprintf(stderr, "\nError: Memory allocation failed\n");
    wait_for_enter();
    return;
  }

  product_count = count;
  product_input_array(products, product_count);

  printf("\n");
  print_separator('-', 60);
  printf("Successfully entered %zu products!\n", product_count);
  print_separator('-', 60);
  wait_for_enter();
}

void action_display_products(void) {
  clear_screen();

  if (!products || product_count == 0) {
    print_separator('=', 60);
    printf("No Products Available\n");
    print_separator('=', 60);
    printf("\nNo products to display. Please input or load products first.\n");
    wait_for_enter();
    return;
  }

  product_display_array(products, product_count, "Product List");
  wait_for_enter();
}

void action_sort_products(void) {
  clear_screen();

  if (!products || product_count == 0) {
    print_separator('=', 60);
    printf("Sort Products\n");
    print_separator('=', 60);
    printf("\nNo products to sort. Please input or load products first.\n");
    wait_for_enter();
    return;
  }

  print_separator('=', 60);
  printf("Sort Products\n");
  print_separator('=', 60);
  printf("\n");

  printf("Select sorting algorithm:\n");
  print_separator('-', 30);
  printf("  [1] Quick Sort\n");
  printf("  [2] Merge Sort\n");
  print_separator('-', 30);
  int algo = get_int_input_range("Enter choice: ", 1, 2);

  printf("\nSelect field to sort by:\n");
  print_separator('-', 30);
  printf("  [1] Name\n");
  printf("  [2] Country\n");
  printf("  [3] Manufacturer\n");
  printf("  [4] Item\n");
  printf("  [5] Price\n");
  print_separator('-', 30);
  int field = get_int_input_range("Enter choice: ", 1, 5);

  printf("\nSelect sort direction:\n");
  print_separator('-', 30);
  printf("  [1] Ascending\n");
  printf("  [2] Descending\n");
  print_separator('-', 30);
  int direction = get_int_input_range("Enter choice: ", 1, 2);

  product_set_sort_direction(direction == 1);

  int (*compare_func)(const void *, const void *) = NULL;
  const char *field_name = "";

  switch (field) {
  case 1:
    compare_func = product_compare_by_name;
    field_name = "Name";
    break;
  case 2:
    compare_func = product_compare_by_country;
    field_name = "Country";
    break;
  case 3:
    compare_func = product_compare_by_manufacturer;
    field_name = "Manufacturer";
    break;
  case 4:
    compare_func = product_compare_by_item;
    field_name = "Item";
    break;
  case 5:
    compare_func = product_compare_by_price;
    field_name = "Price";
    break;
  }

  const char *direction_name = (direction == 1) ? "Ascending" : "Descending";
  char sort_desc[100];
  snprintf(sort_desc, sizeof(sort_desc), "by %s (%s)", field_name, direction_name);

  printf("\n");
  print_separator('=', 60);
  if (algo == 1) {
    printf("Sorting with Quick Sort %s...\n", sort_desc);
    quick_sort(products, product_count, sizeof(Product), compare_func);
  } else {
    printf("Sorting with Merge Sort %s...\n", sort_desc);
    merge_sort(products, product_count, sizeof(Product), compare_func);
  }
  printf("Sorting completed!\n");
  print_separator('=', 60);
  wait_for_enter();
}

void action_load_mock_data(void) {
  clear_screen();
  print_separator('=', 60);
  printf("Loading Mock Data\n");
  print_separator('=', 60);
  printf("\n");

  if (products) {
    free(products);
  }

  product_count = product_create_mock_data(&products);

  if (product_count == 0) {
    fprintf(stderr, "Error: Failed to create mock data\n");
    wait_for_enter();
    return;
  }

  printf("Successfully loaded %zu mock products!\n", product_count);
  print_separator('-', 60);
  wait_for_enter();
}
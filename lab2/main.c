#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/menu.h"
#include "common/product.h"
#include "common/utils.h"

#include "lib/sort.h"

static ProductArray *products = NULL;

void action_input_products(void);
void action_display_products(void);
void action_sort_products(void);
void action_load_mock_data(void);
void action_clear_products(void);

int main(void) {
  enter_alternate_screen();
  setup_terminal_cleanup();

  products = product_array_create(10);
  if (!products) {
    fprintf(stderr, "Error: Failed to initialize product storage\n");
    return 1;
  }

  MenuItem items[] = {{1, "Input products from keyboard", action_input_products},
                      {2, "Display products", action_display_products},
                      {3, "Sort products (Quick Sort / Merge Sort)", action_sort_products},
                      {4, "Load mock data (for testing)", action_load_mock_data},
                      {5, "Clear all products", action_clear_products},
                      {0, "Exit", NULL}};

  int item_count = sizeof(items) / sizeof(MenuItem);

  Menu *main_menu = menu_create("LAB 2 - Product Management System", items, item_count);

  if (!main_menu) {
    fprintf(stderr, "Error: Failed to create menu\n");
    product_array_free(products);
    return 1;
  }

  menu_run(main_menu, 0);

  menu_free(main_menu);

  product_array_free(products);

  return 0;
}

void action_input_products(void) {
  clear_screen();
  print_separator('=', 60);
  printf("Add Products from Keyboard\n");
  print_separator('=', 60);
  printf("\n");

  int count = get_int_input_range("How many products do you want to add? ", 1, 100);

  for (int i = 0; i < count; i++) {
    Product product = {0};
    printf("\n--- Product %d of %d ---\n", i + 1, count);
    product_input(&product);

    if (!product_array_add(products, &product)) {
      fprintf(stderr, "\nError: Could not add product %d\n", i + 1);
      break;
    }
  }

  printf("\n");
  print_separator('-', 60);
  printf("Total products: %zu\n", products->count);
  print_separator('-', 60);
}

void action_display_products(void) {
  clear_screen();

  if (!products || products->count == 0) {
    print_separator('=', 60);
    printf("No Products Available\n");
    print_separator('=', 60);
    printf("\nNo products to display. Please input or load products first.\n");
    return;
  }

  product_display_array(products->items, products->count, "Product List");
}

void action_sort_products(void) {
  clear_screen();

  if (!products || products->count == 0) {
    print_separator('=', 60);
    printf("Sort Products\n");
    print_separator('=', 60);
    printf("\nNo products to sort. Please input or load products first.\n");
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
  const ProductFieldInfo *fields = product_get_sort_fields();
  for (int i = 0; i < PRODUCT_SORT_FIELD_COUNT; i++) {
    printf("  [%d] %s\n", i + 1, fields[i].name);
  }
  print_separator('-', 30);
  int field = get_int_input_range("Enter choice: ", 1, PRODUCT_SORT_FIELD_COUNT);

  printf("\nSelect sort direction:\n");
  print_separator('-', 30);
  printf("  [1] Ascending\n");
  printf("  [2] Descending\n");
  print_separator('-', 30);
  int direction = get_int_input_range("Enter choice: ", 1, 2);

  product_set_sort_direction(direction == 1);

  const ProductFieldInfo *selected_field = &fields[field - 1];
  const char *direction_name = (direction == 1) ? "Ascending" : "Descending";
  char sort_desc[100];
  snprintf(sort_desc, sizeof(sort_desc), "by %s (%s)", selected_field->name, direction_name);

  printf("\n");
  print_separator('=', 60);
  if (algo == 1) {
    printf("Sorting with Quick Sort %s...\n", sort_desc);
    quick_sort(products->items, products->count, sizeof(Product), selected_field->compare);
  } else {
    printf("Sorting with Merge Sort %s...\n", sort_desc);
    merge_sort(products->items, products->count, sizeof(Product), selected_field->compare);
  }
  printf("Sorting completed!\n");
  print_separator('=', 60);
}

void action_load_mock_data(void) {
  clear_screen();
  print_separator('=', 60);
  printf("Loading Mock Data\n");
  print_separator('=', 60);
  printf("\n");

  int mode = get_yes_no_input("Add mock products to the existing list? No will replace the current list.", 1) ? 1 : 2;

  Product *mock_products = NULL;
  size_t mock_count = product_create_mock_data(&mock_products);

  if (mock_count == 0 || !mock_products) {
    fprintf(stderr, "Error: Failed to create mock data\n");
    return;
  }

  if (mode == 2) {
    product_array_clear(products);
  }

  for (size_t i = 0; i < mock_count; i++) {
    if (!product_array_add(products, &mock_products[i])) {
      fprintf(stderr, "Warning: Could not add mock product %zu\n", i + 1);
      break;
    }
  }

  free(mock_products);

  printf("Successfully processed %zu mock products! Total now: %zu\n", mock_count, products->count);
  print_separator('-', 60);
}

void action_clear_products(void) {
  clear_screen();
  print_separator('=', 60);
  printf("Clear Products\n");
  print_separator('=', 60);
  printf("\n");

  if (!products || products->count == 0) {
    printf("No products to clear.\n");
    return;
  }

  if (get_yes_no_input("Clear all products?", 0)) {
    product_array_clear(products);
    printf("All products cleared.\n");
  } else {
    printf("Cancelled.\n");
  }
}
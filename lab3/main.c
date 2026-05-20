#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "common/menu.h"
#include "common/utils.h"

#include "lib/file_ops.h"

static char current_filename[256] = {0};

void action_create_file(void);
void action_display_file(void);
void action_append_product(void);
void action_update_product(void);
void action_search_product(void);
void action_sort_file(void);
void action_delete_product(void);
void action_delete_file(void);

int main(void) {
  enter_alternate_screen();
  setup_terminal_cleanup();

  MenuItem items[] = {{1, "Create binary file and input products", action_create_file},
                      {2, "Display file contents", action_display_file},
                      {3, "Append product to file", action_append_product},
                      {4, "Modify product in file", action_update_product},
                      {5, "Search product in file (Linear Search)", action_search_product},
                      {6, "Sort products in file", action_sort_file},
                      {7, "Delete product from file", action_delete_product},
                      {8, "Delete file from disk", action_delete_file},
                      {0, "Exit", NULL}};

  int item_count = sizeof(items) / sizeof(MenuItem);

  Menu *main_menu = menu_create("Lab3 - Binary File Operations with Products", items, item_count);
  if (!main_menu) {
    fprintf(stderr, "Error: Failed to create menu\n");
    return 1;
  }

  menu_run(main_menu, 0);

  menu_free(main_menu);

  return 0;
}

void action_create_file(void) {
  clear_screen();
  printf("Enter filename: ");
  if (!fgets(current_filename, sizeof(current_filename), stdin)) {
    current_filename[0] = '\0';
  } else {
    current_filename[strcspn(current_filename, "\n")] = '\0';
  }

  if (current_filename[0] == '\0') {
    printf("No filename entered.\n");
    return;
  }

  if (access(current_filename, F_OK) == 0) {
    if (get_yes_no_input("File already exists. Read its contents instead of overwriting?", 1)) {
      printf("\nFile: %s\n", current_filename);
      print_separator('=', 60);
      file_read_and_display(current_filename);
      return;
    }
  }

  ProductArray *products = NULL;
  if (file_create_and_populate(current_filename, &products)) {
    if (products) {
      product_array_free(products);
    }
  } else {
    printf("Failed to create file\n");
  }
}

void action_display_file(void) {
  clear_screen();
  if (current_filename[0] == '\0') {
    printf("No file loaded. Use option 1 to create a file first.\n");
    return;
  }

  printf("File: %s\n", current_filename);
  print_separator('=', 60);
  file_read_and_display(current_filename);
}

void action_append_product(void) {
  clear_screen();
  if (current_filename[0] == '\0') {
    printf("No file loaded. Use option 1 to create a file first.\n");
    return;
  }

  printf("File: %s\n", current_filename);
  print_separator('=', 60);

  Product product = {0};
  printf("\nEnter product details:\n");
  product_input(&product);

  if (!file_append_product(current_filename, &product)) {
    printf("Failed to append product\n");
  }
}

void action_update_product(void) {
  clear_screen();
  if (current_filename[0] == '\0') {
    printf("No file loaded. Use option 1 to create a file first.\n");
    return;
  }

  printf("File: %s\n", current_filename);
  print_separator('=', 60);

  // Load the file to validate the index
  FILE *file = fopen(current_filename, "rb");
  if (!file) {
    printf("Cannot open file to validate index\n");
    return;
  }

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  size_t product_count = file_size / sizeof(Product);
  fclose(file);

  printf("File contains %zu products\n", product_count);

  if (product_count == 0) {
    printf("File is empty, nothing to modify\n");
    return;
  }

  int idx = get_int_input_range("Enter product index to modify (0-based): ", 0, (int)product_count - 1);
  size_t index = (size_t)idx;

  Product product = {0};
  printf("\nEnter new product details:\n");
  product_input(&product);

  if (!file_update_product(current_filename, index, &product)) {
    printf("Failed to update product\n");
  }
}

void action_search_product(void) {
  clear_screen();
  if (current_filename[0] == '\0') {
    printf("No file loaded. Use option 1 to create a file first.\n");
    return;
  }

  printf("File: %s\n", current_filename);
  print_separator('=', 60);

  printf("\nSearch by field:\n");
  const ProductFieldInfo *fields = product_get_sort_fields();
  for (int i = 0; i < PRODUCT_SORT_FIELD_COUNT; i++) {
    printf("%d. %s\n", i + 1, fields[i].name);
  }

  int field_index = get_int_input_range("Select field: ", 1, PRODUCT_SORT_FIELD_COUNT) - 1;

  char search_value[100] = {0};
  printf("Enter search value: ");
  fgets(search_value, sizeof(search_value), stdin);
  search_value[strcspn(search_value, "\n")] = '\0';

  file_search_by_field(current_filename, field_index, search_value);
}

void action_sort_file(void) {
  clear_screen();
  if (current_filename[0] == '\0') {
    printf("No file loaded. Use option 1 to create a file first.\n");
    return;
  }

  printf("File: %s\n", current_filename);
  print_separator('=', 60);

  printf("\nSort by field:\n");
  const ProductFieldInfo *fields = product_get_sort_fields();
  for (int i = 0; i < PRODUCT_SORT_FIELD_COUNT; i++) {
    printf("%d. %s\n", i + 1, fields[i].name);
  }

  int field_index = get_int_input_range("Select field: ", 1, PRODUCT_SORT_FIELD_COUNT) - 1;

  printf("\nSort direction:\n");
  printf("0. Ascending\n");
  printf("1. Descending\n");
  int direction = get_int_input("Select direction: ");
  if (direction != 0)
    direction = 1;

  if (!file_sort_by_field(current_filename, field_index, direction == 0 ? 1 : -1)) {
    printf("Failed to sort file\n");
  }
}

void action_delete_product(void) {
  clear_screen();
  if (current_filename[0] == '\0') {
    printf("No file loaded. Use option 1 to create a file first.\n");
    return;
  }

  printf("File: %s\n", current_filename);
  print_separator('=', 60);

  // Load the file to validate the index
  FILE *file = fopen(current_filename, "rb");
  if (!file) {
    printf("Cannot open file to validate index\n");
    return;
  }

  // Count products in file
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  size_t product_count = file_size / sizeof(Product);
  fclose(file);

  printf("File contains %zu products\n", product_count);

  if (product_count == 0) {
    printf("File is empty, nothing to delete\n");
    return;
  }

  int idx = get_int_input_range("Enter product index to delete (0-based): ", 0, (int)product_count - 1);
  size_t index = (size_t)idx;

  if (!file_delete_product(current_filename, index)) {
    printf("Failed to delete product\n");
  }
}

void action_delete_file(void) {
  clear_screen();
  if (current_filename[0] == '\0') {
    printf("No file currently loaded\n");
    return;
  }

  printf("File: %s\n", current_filename);
  print_separator('=', 60);

  if (get_yes_no_input("Are you sure you want to delete the file?: ", 0)) {
    if (file_delete_file(current_filename)) {
      current_filename[0] = '\0';
    } else {
      printf("Failed to delete file\n");
    }
  } else {
    printf("Cancelled\n");
  }
}
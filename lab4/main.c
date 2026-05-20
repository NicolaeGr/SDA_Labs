#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "common/menu.h"
#include "common/product/product.h"
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

  MenuItem items[] = {{1, "Create text file and input products", action_create_file},
                      {2, "Display file contents", action_display_file},
                      {3, "Append product to file", action_append_product},
                      {4, "Modify product in file", action_update_product},
                      {5, "Search product in file (Linear Search)", action_search_product},
                      {6, "Sort products in file (Insertion Sort)", action_sort_file},
                      {7, "Delete product from file", action_delete_product},
                      {8, "Delete file from disk", action_delete_file},
                      {0, "Exit", NULL}};

  int item_count = sizeof(items) / sizeof(MenuItem);
  Menu *main_menu = menu_create("Lab4 - Text File Operations with Products", items, item_count);
  if (!main_menu) {
    fprintf(stderr, "Error: Failed to create menu\n");
    return 1;
  }

  menu_run(main_menu, 0);
  menu_free(main_menu);
  return 0;
}

static void read_filename(char *buffer, size_t size) {
  printf("Enter filename: ");
  if (!fgets(buffer, size, stdin)) {
    buffer[0] = '\0';
    return;
  }
  buffer[strcspn(buffer, "\n")] = '\0';
}

static int get_loaded_product_count(const char *filename, size_t *count) {
  if (!filename || !count) {
    return 0;
  }

  ProductArray *products = product_array_create(10);
  if (!products) {
    return 0;
  }

  if (!text_file_load_all(filename, &products)) {
    product_array_free(products);
    return 0;
  }

  *count = products->count;
  product_array_free(products);
  return 1;
}

void action_create_file(void) {
  clear_screen();
  read_filename(current_filename, sizeof(current_filename));

  if (current_filename[0] == '\0') {
    printf("No filename entered.\n");
    return;
  }

  if (access(current_filename, F_OK) == 0) {
    if (get_yes_no_input("File already exists. Read it instead of overwriting?", 1)) {
      printf("\nFile: %s\n", current_filename);
      print_separator('=', 60);
      text_file_read_and_display(current_filename);
      return;
    }
  }

  ProductArray *products = NULL;
  if (text_file_create_and_populate(current_filename, &products)) {
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
  text_file_read_and_display(current_filename);
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

  if (!text_file_append_product(current_filename, &product)) {
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

  size_t product_count = 0;
  if (!get_loaded_product_count(current_filename, &product_count)) {
    printf("Failed to load file data.\n");
    return;
  }

  if (product_count == 0) {
    printf("File is empty, nothing to modify.\n");
    return;
  }

  int idx = get_int_input_range("Enter product index to modify: ", 0, (int)product_count - 1);
  size_t index = (size_t)idx;

  Product product = {0};
  printf("\nEnter new product details:\n");
  product_input(&product);

  if (!text_file_update_product(current_filename, index, &product)) {
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
  if (!fgets(search_value, sizeof(search_value), stdin)) {
    printf("Failed to read search value\n");
    return;
  }
  search_value[strcspn(search_value, "\n")] = '\0';

  text_file_search_by_field(current_filename, field_index, search_value);
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
  if (direction != 0) {
    direction = 1;
  }

  if (!text_file_sort_by_field(current_filename, field_index, direction == 0 ? 1 : -1)) {
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

  size_t product_count = 0;
  if (!get_loaded_product_count(current_filename, &product_count)) {
    printf("Failed to load file data.\n");
    return;
  }

  if (product_count == 0) {
    printf("File is empty, nothing to delete.\n");
    return;
  }

  int idx = get_int_input_range("Enter product index to delete: ", 0, (int)product_count - 1);
  size_t index = (size_t)idx;

  if (!text_file_delete_product(current_filename, index)) {
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
    if (text_file_delete_file(current_filename)) {
      current_filename[0] = '\0';
    } else {
      printf("Failed to delete file\n");
    }
  } else {
    printf("Cancelled\n");
  }
}

#include <stdio.h>

#include "common/menu.h"
#include "common/product/product.h"
#include "common/utils.h"

#include "lib/stack_queue.h"

static ProductStack stack_data;
static ProductQueue queue_data;
static int stack_created = 0;
static Product last_modified;
static int has_modified = 0;

static int choose_file_mode(void);
static void action_create_stack(void);
static void action_read_stack_data(void);
static void action_write_stack_file(void);
static void action_find_max(void);
static void action_copy_stack_to_queue(void);
static void action_write_queue_file(void);
static void action_stack_length(void);
static void action_modify_stack_element(void);
static void action_append_modified_to_stack_file(void);
static void action_free_stack(void);
static void action_free_queue(void);

int main(void) {
  enter_alternate_screen();
  setup_terminal_cleanup();

  stack_init(&stack_data);
  queue_init(&queue_data);

  MenuItem items[] = {
      {1, "Create dynamic stack", action_create_stack},
      {2, "Read stack elements from keyboard", action_read_stack_data},
      {3, "Write stack to stiva.txt/stiva.bin", action_write_stack_file},
      {4, "Find maximal element by numeric field", action_find_max},
      {5, "Copy stack to queue", action_copy_stack_to_queue},
      {6, "Write queue to queue.txt/queue.bin", action_write_queue_file},
      {7, "Determine stack length", action_stack_length},
      {8, "Modify stack element fields", action_modify_stack_element},
      {9, "Append modified element to stack file", action_append_modified_to_stack_file},
      {10, "Free stack memory", action_free_stack},
      {11, "Free queue memory", action_free_queue},
      {0, "Exit", NULL},
  };

  int item_count = sizeof(items) / sizeof(MenuItem);
  Menu *menu = menu_create("LAB 5 - Stack and Queue", items, item_count);
  if (!menu) {
    fprintf(stderr, "Failed to create menu\n");
    return 1;
  }

  menu_run(menu, 0);
  menu_free(menu);

  stack_free(&stack_data);
  queue_free(&queue_data);
  return 0;
}

static int choose_file_mode(void) {
  printf("1. Text file\n");
  printf("2. Binary file\n");
  return get_int_input_range("Choose file mode: ", 1, 2);
}

static void action_create_stack(void) {
  if (stack_created) {
    stack_free(&stack_data);
  }
  stack_init(&stack_data);
  stack_created = 1;
  has_modified = 0;
  printf("Dynamic stack created.\n");
}

static void action_read_stack_data(void) {
  if (!stack_created) {
    printf("Create stack first (option 1).\n");
    return;
  }

  int count = get_int_input_range("How many elements to push? ", 1, 1000);
  for (int i = 0; i < count; i++) {
    Product item = {0};
    printf("\n--- Stack element %d/%d ---\n", i + 1, count);
    product_input(&item);
    if (!stack_push(&stack_data, &item)) {
      printf("Failed to push item into stack.\n");
      return;
    }
  }

  printf("Inserted %d elements into stack.\n", count);
}

static void action_write_stack_file(void) {
  if (!stack_created) {
    printf("Create stack first (option 1).\n");
    return;
  }

  int mode = choose_file_mode();
  const char *filename = (mode == 1) ? "stiva.txt" : "stiva.bin";
  const char *open_mode = (mode == 1) ? "w" : "wb";

  FILE *file = fopen(filename, open_mode);
  if (!file) {
    perror("Failed to open stack file");
    return;
  }

  int ok = (mode == 1) ? stack_write_text(&stack_data, file) : stack_write_binary(&stack_data, file);
  fclose(file);

  if (!ok) {
    printf("Failed writing stack data to file.\n");
    return;
  }

  printf("Stack written to %s\n", filename);
}

static void action_find_max(void) {
  if (!stack_created || stack_size(&stack_data) == 0) {
    printf("Stack is empty.\n");
    return;
  }

  const Product *max_item = stack_find_max_price(&stack_data);
  if (!max_item) {
    printf("Could not determine max element.\n");
    return;
  }

  printf("Element with maximal numeric field (price):\n");
  product_display(max_item);
}

static void action_copy_stack_to_queue(void) {
  if (!stack_created) {
    printf("Create stack first (option 1).\n");
    return;
  }

  if (!stack_copy_to_queue(&stack_data, &queue_data)) {
    printf("Failed to copy stack to queue.\n");
    return;
  }

  printf("Copied %zu elements from stack to queue.\n", queue_data.size);
}

static void action_write_queue_file(void) {
  int mode = choose_file_mode();
  const char *filename = (mode == 1) ? "queue.txt" : "queue.bin";
  const char *open_mode = (mode == 1) ? "w" : "wb";

  FILE *file = fopen(filename, open_mode);
  if (!file) {
    perror("Failed to open queue file");
    return;
  }

  int ok = (mode == 1) ? queue_write_text(&queue_data, file) : queue_write_binary(&queue_data, file);
  fclose(file);

  if (!ok) {
    printf("Failed writing queue data to file.\n");
    return;
  }

  printf("Queue written to %s\n", filename);
}

static void action_stack_length(void) {
  if (!stack_created) {
    printf("Create stack first (option 1).\n");
    return;
  }

  printf("Stack length: %zu\n", stack_size(&stack_data));
}

static void action_modify_stack_element(void) {
  if (!stack_created || stack_size(&stack_data) == 0) {
    printf("Stack is empty.\n");
    return;
  }

  int idx = get_int_input_range("Element position from top (0-based): ", 0, (int)stack_size(&stack_data) - 1);
  Product *target = stack_get_at(&stack_data, (size_t)idx);
  if (!target) {
    printf("Invalid position.\n");
    return;
  }

  printf("Enter new values for selected element:\n");
  product_input(target);

  last_modified = *target;
  has_modified = 1;
  printf("Element modified successfully.\n");
}

static void action_append_modified_to_stack_file(void) {
  if (!has_modified) {
    printf("No modified element available. Use option 8 first.\n");
    return;
  }

  int mode = choose_file_mode();
  const char *filename = (mode == 1) ? "stiva.txt" : "stiva.bin";
  const char *open_mode = (mode == 1) ? "a" : "ab";

  FILE *file = fopen(filename, open_mode);
  if (!file) {
    perror("Failed to open stack file");
    return;
  }

  int ok;
  if (mode == 1) {
    ok = fprintf(file, "APPENDED: %s | %s | %s | %s | %.2f\n", last_modified.name, last_modified.country,
                 last_modified.manufacturer, last_modified.item_code, last_modified.price) >= 0;
  } else {
    ok = fwrite(&last_modified, sizeof(Product), 1, file) == 1;
  }

  fclose(file);

  if (!ok) {
    printf("Failed to append modified element.\n");
    return;
  }

  printf("Modified element appended to %s\n", filename);
}

static void action_free_stack(void) {
  if (!stack_created) {
    printf("Stack was not created yet.\n");
    return;
  }

  stack_free(&stack_data);
  stack_created = 0;
  has_modified = 0;
  printf("Stack memory released.\n");
}

static void action_free_queue(void) {
  queue_free(&queue_data);
  printf("Queue memory released.\n");
}

#include "menu.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  MenuItem items[] = {{0, "Exit", NULL}};

  int item_count = sizeof(items) / sizeof(MenuItem);

  Menu *main_menu = menu_create("LAB 2 - NOT IMPLEMENTED", items, item_count);

  if (!main_menu) {
    fprintf(stderr, "Error: Failed to create menu\n");
    return 1;
  }

  menu_run(main_menu, 0);

  menu_free(main_menu);

  return 0;
}

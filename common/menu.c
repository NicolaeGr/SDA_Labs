#include "menu.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

Menu *menu_create(const char *title, MenuItem *items, int item_count) {
  Menu *menu = (Menu *)malloc(sizeof(Menu));
  if (!menu) {
    return NULL;
  }

  menu->title = title;
  menu->items = items;
  menu->item_count = item_count;

  return menu;
}

void menu_display(Menu *menu) {
  if (!menu) {
    return;
  }

  printf("\n");
  print_separator('=', 60);
  printf("%s\n", menu->title);
  print_separator('=', 60);

  for (int i = 0; i < menu->item_count; i++) {
    printf("  [%d] %s\n", menu->items[i].id, menu->items[i].label);
  }

  print_separator('-', 60);
}

void menu_run(Menu *menu, int exit_id) {
  if (!menu) {
    return;
  }

  int choice;
  int found;

  while (1) {
    sync_terminal_events();
    consume_resume_to_menu_request();
    clear_screen();
    menu_display(menu);

    choice = get_int_input("Enter your choice: ");

    if (choice == exit_id) {
      printf("\nExiting...\n");
      break;
    }

    found = 0;
    for (int i = 0; i < menu->item_count; i++) {
      if (menu->items[i].id == choice) {
        found = 1;
        if (menu->items[i].action) {
          clear_screen();
          int completed = run_menu_action_with_resume_guard(menu->items[i].action);
          if (!completed || consume_resume_to_menu_request()) {
            break;
          }
          wait_for_enter();
        }
        break;
      }
    }

    if (!found) {
      printf("\nInvalid choice! Please try again.\n");
      wait_for_enter();
    }
  }
}

void menu_free(Menu *menu) {
  if (menu) {
    free(menu);
  }
}

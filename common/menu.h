#ifndef MENU_H
#define MENU_H

typedef struct
{
    int id;
    const char *label;
    void (*action)(void);
} MenuItem;

typedef struct
{
    const char *title;
    MenuItem *items;
    int item_count;
} Menu;

Menu *menu_create(const char *title, MenuItem *items, int item_count);

void menu_display(Menu *menu);

void menu_run(Menu *menu, int exit_id);

void menu_free(Menu *menu);

#endif // MENU_H
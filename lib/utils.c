#include "utils.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void clear_screen(void) {
  printf("\033[2J\033[H");
  fflush(stdout);
}

void print_separator(char ch, int length) {
  for (int i = 0; i < length; i++) {
    putchar(ch);
  }
  putchar('\n');
}

void wait_for_enter(void) {
  printf("\nPress Enter to continue...");
  fflush(stdout);

  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

int get_int_input(const char *prompt) {
  int value;
  int result;

  while (1) {
    printf("%s", prompt);
    fflush(stdout);

    result = scanf("%d", &value);

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;

    if (result == 1) {
      return value;
    }

    printf("Invalid input! Please enter a valid integer.\n");
  }
}

int get_int_input_range(const char *prompt, int min, int max) {
  int value;

  while (1) {
    value = get_int_input(prompt);

    if (value >= min && value <= max) {
      return value;
    }

    printf("Value must be between %d and %d. Please try again.\n", min, max);
  }
}

int get_yes_no_input(const char *prompt, int default_yes) {
  char input[10];

  while (1) {
    if (default_yes) {
      printf("%s (Yes/no): ", prompt);
    } else {
      printf("%s (yes/No): ", prompt);
    }
    fflush(stdout);

    if (fgets(input, sizeof(input), stdin) == NULL) {
      continue;
    }

    input[strcspn(input, "\n")] = 0;

    if (input[0] == '\0') {
      return default_yes ? 1 : 0;
    }

    char first = input[0];
    if (first == 'y' || first == 'Y') {
      return 1;
    } else if (first == 'n' || first == 'N') {
      return 0;
    }

    printf("Invalid input! Please enter 'y' or 'n'.\n");
  }
}

void print_vec1(const int *arr, size_t size, const char *title) {
  if (title) {
    printf("\n%s\n", title);
    print_separator('-', 60);
  }

  printf("[ ");
  for (size_t i = 0; i < size; i++) {
    printf("%d", arr[i]);
    if (i < size - 1) {
      printf(", ");
    }
  }
  printf(" ]\n");
}

void print_vec2(const int *arr, size_t rows, size_t cols, const char *title) {
  if (title) {
    printf("\n%s\n", title);
    print_separator('-', 60);
  }

  int max_width = 0;
  for (size_t i = 0; i < rows * cols; i++) {
    int width = snprintf(NULL, 0, "%d", arr[i]);
    if (width > max_width) {
      max_width = width;
    }
  }
  max_width += 2;

  for (size_t i = 0; i < rows; i++) {
    printf("| ");
    for (size_t j = 0; j < cols; j++) {
      printf("%*d ", max_width, arr[i * cols + j]);
    }
    printf("|\n");
  }
}

void input_vec1(int *arr, size_t size) {
  printf("\nEnter %zu elements:\n", size);
  for (size_t i = 0; i < size; i++) {
    char prompt[50];
    snprintf(prompt, sizeof(prompt), "Element [%zu]: ", i);
    arr[i] = get_int_input(prompt);
  }
}

void input_vec2(int *arr, size_t rows, size_t cols) {
  printf("\nEnter elements for %zu x %zu matrix:\n", rows, cols);
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      char prompt[50];
      snprintf(prompt, sizeof(prompt), "Element [%zu][%zu]: ", i, j);
      arr[i * cols + j] = get_int_input(prompt);
    }
  }
}

void generate_random_vec1(int *arr, size_t size, int min, int max) {
  static int seeded = 0;
  if (!seeded) {
    srand((unsigned int)time(NULL));
    seeded = 1;
  }

  int range = max - min + 1;
  for (size_t i = 0; i < size; i++) {
    arr[i] = min + (rand() % range);
  }
}

void generate_random_vec2(int *arr, size_t rows, size_t cols, int min, int max) {
  generate_random_vec1(arr, rows * cols, min, max);
}

void copy_vec1(int *dest, const int *src, size_t size) { memcpy(dest, src, size * sizeof(int)); }

void copy_vec2(int *dest, const int *src, size_t rows, size_t cols) { memcpy(dest, src, rows * cols * sizeof(int)); }

int *duplicate_vec1(const int *src, size_t size) {
  int *copy = (int *)malloc(size * sizeof(int));
  if (copy) {
    memcpy(copy, src, size * sizeof(int));
  }
  return copy;
}

int *duplicate_vec2(const int *src, size_t rows, size_t cols) { return duplicate_vec1(src, rows * cols); }

void print_sorting_mode(void) {
  printf("\n[Mode: ");
  if (g_use_by_value_mode) {
    printf("Version A - Pass by Value (Creates Copy)");
  } else {
    printf("Version B - Pass by Pointer (In-Place)");
  }
  printf("]\n");
}

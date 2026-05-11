#include <stdio.h>
#include <stdlib.h>

#include "common/menu.h"
#include "common/utils.h"

#include "lib/array_utils.h"
#include "lib/globals.h"
#include "lib/sort/sort.h"

int g_use_pointer_arithmetic_mode = 1;

#define K_CONSTANT 5

typedef void (*sort_fn)(int *arr, size_t size, int ascending);

void task_1a(void);
void task_1b(void);
void task_1c(void);
void task_2a(void);
void task_2b(void);
void select_mode(void);
void print_sorting_mode(void);
void run_selected_sort(sort_fn array_sort, sort_fn pointer_sort, int *arr, size_t size, int ascending);

int main(void) {
  enter_alternate_screen();
  setup_terminal_cleanup();

  select_mode();
  wait_for_enter();

  MenuItem items[] = {{1, "Task 1A - Array Analysis & HeapSort/CountingSort", task_1a},
                      {2, "Task 1B - Prime Check & RadixSort/CombSort", task_1b},
                      {3, "Task 1C - Negative Product & MergeSort/BubbleSort", task_1c},
                      {4, "Task 2A - 2D Array Diagonal & QuickSort/ShellSort", task_2a},
                      {5, "Task 2B - 2D Array Max Element & SelectionSort/InsertionSort", task_2b},
                      {6, "Change sorting method", select_mode},
                      {0, "Exit", NULL}};

  int item_count = sizeof(items) / sizeof(MenuItem);

  Menu *main_menu = menu_create("LAB 1 - SORTING ALGORITHMS", items, item_count);

  if (!main_menu) {
    fprintf(stderr, "Error: Failed to create menu\n");
    return 1;
  }

  menu_run(main_menu, 0);

  menu_free(main_menu);

  return 0;
}

void print_sorting_mode(void) {
  printf("\n[Indexing method: ");
  if (g_use_pointer_arithmetic_mode) {
    printf("Pointer arithmetic ((arr + i))");
  } else {
    printf("Array notation (arr[i])");
  }
  printf("]\n");
}

void select_mode(void) {
  clear_screen();
  printf("\nSORTING METHOD\n");
  print_separator('=', 40);
  printf("[1] Array notation (arr[i])\n");
  printf("[2] Pointer arithmetic (*(arr + i))\n");

  int choice = get_int_input_range("\nChoice: ", 1, 2);
  g_use_pointer_arithmetic_mode = (choice == 2) ? 1 : 0;

  printf("Method: %s\n", g_use_pointer_arithmetic_mode ? "Pointer arithmetic" : "Array notation");
}

void run_selected_sort(sort_fn array_sort, sort_fn pointer_sort, int *arr, size_t size, int ascending) {
  if (g_use_pointer_arithmetic_mode) {
    pointer_sort(arr, size, ascending);
  } else {
    array_sort(arr, size, ascending);
  }
}

void task_1a(void) {
  printf("TASK 1A: Sort by average comparison (HeapSort/CountingSort)\n");
  print_separator('=', 60);

  int n = get_int_input_range("\nArray size: ", 0, 1000);
  int *arr = create_and_populate_vec1(n, -100, 100);

  print_vec1(arr, n, "Original Array:");
  print_sorting_mode();

  if (n == 0) {
    printf("\nEmpty array --> no sorting to apply.\n");
    print_vec1(arr, n, "\nSorted Array:");
    free(arr);
    return;
  }

  double avg_even = calculate_avg_at_even_positions(arr, n);
  double avg_odd = calculate_avg_at_odd_positions(arr, n);
  int ascending;
  sort_fn array_sort;
  sort_fn pointer_sort;

  printf("\nAvg[even]=%.2f, Avg[odd]=%.2f\n", avg_even, avg_odd);

  if (avg_even > avg_odd) {
    printf("HeapSort (ascending)\n");
    array_sort = heap_sort_array;
    pointer_sort = heap_sort_pointer;
    ascending = 1;
  } else {
    printf("CountingSort (descending)\n");
    array_sort = counting_sort_array;
    pointer_sort = counting_sort_pointer;
    ascending = 0;
  }

  run_selected_sort(array_sort, pointer_sort, arr, (size_t)n, ascending);

  print_vec1(arr, n, "\nSorted Array:");

  free(arr);
}

void task_1b(void) {
  printf("TASK 1B: Sort by prime check (RadixSort/CombSort)\n");
  print_separator('=', 60);

  int n = get_int_input_range("\nArray size: ", 0, 1000);
  int *arr = create_and_populate_vec1(n, 1, 100);

  print_vec1(arr, n, "Original Array:");
  print_sorting_mode();

  if (n == 0) {
    printf("\nEmpty array --> no sorting to apply.\n");
    print_vec1(arr, n, "\nSorted Array:");
    free(arr);
    return;
  }

  int has_primes = has_prime_numbers(arr, n);
  int ascending;
  sort_fn array_sort;
  sort_fn pointer_sort;

  if (has_primes) {
    printf("\nPrimes found --> RadixSort (ascending)\n");
    array_sort = radix_sort_array;
    pointer_sort = radix_sort_pointer;
    ascending = 1;
  } else {
    printf("\nNo primes --> CombSort (descending)\n");
    array_sort = comb_sort_array;
    pointer_sort = comb_sort_pointer;
    ascending = 0;
  }

  run_selected_sort(array_sort, pointer_sort, arr, (size_t)n, ascending);

  print_vec1(arr, n, "\nSorted Array:");

  free(arr);
}

void task_1c(void) {
  printf("TASK 1C: Sort by negative product (MergeSort/BubbleSort)\n");
  print_separator('=', 60);

  int n = get_int_input_range("\nArray size: ", 0, 1000);
  int *arr = create_and_populate_vec1(n, -50, 50);

  print_vec1(arr, n, "Original Array:");
  print_sorting_mode();

  if (n == 0) {
    printf("\nEmpty array --> no sorting to apply.\n");
    print_vec1(arr, n, "\nSorted Array:");
    free(arr);
    return;
  }

  int product;
  int has_negatives = calculate_product_of_negatives(arr, n, &product);
  int ascending;
  sort_fn array_sort;
  sort_fn pointer_sort;

  if (has_negatives && product < 0) {
    printf("\nNegative product=%d --> MergeSort (descending)\n", product);
    array_sort = merge_sort_array;
    pointer_sort = merge_sort_pointer;
    ascending = 0;
  } else {
    if (!has_negatives) {
      printf("\nNo negative elements --> BubbleSort (ascending)\n");
    } else {
      printf("\nNegative product is not < 0 --> BubbleSort (ascending)\n");
    }
    array_sort = bubble_sort_array;
    pointer_sort = bubble_sort_pointer;
    ascending = 1;
  }

  run_selected_sort(array_sort, pointer_sort, arr, (size_t)n, ascending);

  print_vec1(arr, n, "\nSorted Array:");

  free(arr);
}

void task_2a(void) {
  printf("TASK 2A: Diagonal/column sort (QuickSort/ShellSort)\n");
  print_separator('=', 60);

  int n = get_int_input_range("\nMatrix size (n x n): ", 0, 100);
  int *arr = create_and_populate_vec2(n, n, -50, 50);

  print_vec2(arr, n, n, "Original Array:");
  print_sorting_mode();

  if (n == 0) {
    printf("\nEmpty matrix --> no sorting to apply.\n");
    print_vec2(arr, n, n, "\nSorted Array:");
    free(arr);
    return;
  }

  int count = count_above_main_diagonal(n);
  printf("\nAbove diagonal=%d, k=%d\n", count, K_CONSTANT);

  if (count > K_CONSTANT) {
    printf("QuickSort secondary diagonal (ascending)\n");
    int *diag = (int *)malloc(n * sizeof(int));

    extract_secondary_diagonal(arr, n, diag);

    run_selected_sort(quick_sort_array, quick_sort_pointer, diag, (size_t)n, 1);
    place_secondary_diagonal(arr, n, diag);

    free(diag);
    print_vec2(arr, n, n, "\nSorted Array:");
  } else {
    printf("ShellSort first column (descending)\n");
    int *col = (int *)malloc(n * sizeof(int));

    extract_first_column(arr, n, n, col);

    run_selected_sort(shell_sort_array, shell_sort_pointer, col, (size_t)n, 0);
    place_first_column(arr, n, n, col);

    free(col);
    print_vec2(arr, n, n, "\nSorted Array:");
  }

  free(arr);
}

void task_2b(void) {
  printf("TASK 2B: Sort by max element (SelectionSort/InsertionSort)\n");
  print_separator('=', 60);

  int n = get_int_input_range("\nMatrix size (n x n): ", 0, 100);
  int rows = n;
  int cols = n;
  int *arr = create_and_populate_vec2(n, n, -50, 50);

  print_vec2(arr, rows, cols, "Original Array:");
  print_sorting_mode();

  if (rows == 0 || cols == 0) {
    printf("\nEmpty matrix --> no max element and no sorting to apply.\n");
    free(arr);
    return;
  }

  int max_val, count;
  int *positions = NULL;
  find_max_info(arr, rows, cols, &max_val, &count, &positions);

  printf("\nMax=%d appears %d time(s)\n", max_val, count);

  if (count == 1) {
    printf("Position [%d][%d] --> SelectionSort row (ascending)\n", positions[0], positions[1]);
    int *row = (int *)malloc(cols * sizeof(int));

    extract_row(arr, cols, positions[0], row);

    run_selected_sort(selection_sort_array, selection_sort_pointer, row, (size_t)cols, 1);
    place_row(arr, cols, positions[0], row);

    free(row);
    print_vec2(arr, rows, cols, "\nSorted Array:");
  } else {
    printf("Multiple occurrences --> InsertionSort columns (descending)\n");

    int *col_indices = (int *)malloc(count * sizeof(int));
    int unique_cols = 0;

    for (int i = 0; i < count; i++) {
      int col_idx = positions[i * 2 + 1];
      int found = 0;
      for (int j = 0; j < unique_cols; j++) {
        if (col_indices[j] == col_idx) {
          found = 1;
          break;
        }
      }
      if (!found) {
        col_indices[unique_cols++] = col_idx;
      }
    }

    for (int i = 0; i < unique_cols; i++) {
      int col_idx = col_indices[i];
      int *col = (int *)malloc(rows * sizeof(int));
      extract_column(arr, rows, cols, col_idx, col);

      run_selected_sort(insertion_sort_array, insertion_sort_pointer, col, (size_t)rows, 0);
      place_column(arr, rows, cols, col_idx, col);

      free(col);
    }

    free(col_indices);
    print_vec2(arr, rows, cols, "\nSorted Array:");
  }

  free(positions);
  free(arr);
}

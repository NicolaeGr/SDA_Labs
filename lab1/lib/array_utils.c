#include <math.h>
#include <stdlib.h>

#include "common/utils.h"

#include "array_utils.h"

void populate_vec1(int *arr, size_t n, int min, int max) {
  if (get_yes_no_input("Generate random array?", 1)) {
    generate_random_vec1(arr, n, min, max);
  } else {
    input_vec1(arr, n);
  }
}

void populate_vec2(int *arr, size_t rows, size_t cols, int min, int max) {
  if (get_yes_no_input("Generate random array?", 1)) {
    generate_random_vec2(arr, rows, cols, min, max);
  } else {
    input_vec2(arr, rows, cols);
  }
}

int *create_and_populate_vec1(size_t n, int min, int max) {
  int *arr = (int *)malloc(n * sizeof(int));
  populate_vec1(arr, n, min, max);
  return arr;
}

int *create_and_populate_vec2(size_t rows, size_t cols, int min, int max) {
  int *arr = (int *)malloc(rows * cols * sizeof(int));
  populate_vec2(arr, rows, cols, min, max);
  return arr;
}

int is_prime(int n) {
  if (n < 2)
    return 0;
  if (n == 2)
    return 1;
  if (n % 2 == 0)
    return 0;

  int sqrt_n = (int)sqrt((double)n);
  for (int i = 3; i <= sqrt_n; i += 2) {
    if (n % i == 0)
      return 0;
  }
  return 1;
}

double calculate_avg_at_even_positions(const int *arr, size_t size) {
  double sum = 0;
  int count = 0;
  for (size_t i = 0; i < size; i += 2) {
    sum += arr[i];
    count++;
  }
  return count > 0 ? sum / count : 0.0;
}

double calculate_avg_at_odd_positions(const int *arr, size_t size) {
  double sum = 0;
  int count = 0;
  for (size_t i = 1; i < size; i += 2) {
    sum += arr[i];
    count++;
  }
  return count > 0 ? sum / count : 0.0;
}

int has_prime_numbers(const int *arr, size_t size) {
  for (size_t i = 0; i < size; i++) {
    if (is_prime(arr[i]))
      return 1;
  }
  return 0;
}

int calculate_product_of_negatives(const int *arr, size_t size, int *product) {
  int prod = 1;
  int has_negative = 0;

  for (size_t i = 0; i < size; i++) {
    if (arr[i] < 0) {
      prod *= arr[i];
      has_negative = 1;
    }
  }

  *product = prod;
  return has_negative;
}

int count_above_main_diagonal(size_t n) {
  int count = 0;
  for (size_t i = 0; i < n; i++) {
    for (size_t j = i + 1; j < n; j++) {
      count++;
    }
  }
  return count;
}

void extract_secondary_diagonal(const int *arr, size_t n, int *diag) {
  for (size_t i = 0; i < n; i++) {
    diag[i] = arr[i * n + (n - 1 - i)];
  }
}

void place_secondary_diagonal(int *arr, size_t n, const int *diag) {
  for (size_t i = 0; i < n; i++) {
    arr[i * n + (n - 1 - i)] = diag[i];
  }
}

void extract_first_column(const int *arr, size_t rows, size_t cols, int *col) {
  for (size_t i = 0; i < rows; i++) {
    col[i] = arr[i * cols];
  }
}

void place_first_column(int *arr, size_t rows, size_t cols, const int *col) {
  for (size_t i = 0; i < rows; i++) {
    arr[i * cols] = col[i];
  }
}

void find_max_info(const int *arr, size_t rows, size_t cols, int *max_val, int *count, int **positions) {
  *max_val = arr[0];
  *count = 0;

  for (size_t i = 0; i < rows * cols; i++) {
    if (arr[i] > *max_val) {
      *max_val = arr[i];
      *count = 1;
    } else if (arr[i] == *max_val) {
      (*count)++;
    }
  }

  *positions = (int *)malloc(*count * 2 * sizeof(int));

  int idx = 0;
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      if (arr[i * cols + j] == *max_val) {
        (*positions)[idx * 2] = i;
        (*positions)[idx * 2 + 1] = j;
        idx++;
      }
    }
  }
}

void extract_row(const int *arr, size_t cols, size_t row_idx, int *row) {
  for (size_t j = 0; j < cols; j++) {
    row[j] = arr[row_idx * cols + j];
  }
}

void place_row(int *arr, size_t cols, size_t row_idx, const int *row) {
  for (size_t j = 0; j < cols; j++) {
    arr[row_idx * cols + j] = row[j];
  }
}

void extract_column(const int *arr, size_t rows, size_t cols, size_t col_idx, int *col) {
  for (size_t i = 0; i < rows; i++) {
    col[i] = arr[i * cols + col_idx];
  }
}

void place_column(int *arr, size_t rows, size_t cols, size_t col_idx, const int *col) {
  for (size_t i = 0; i < rows; i++) {
    arr[i * cols + col_idx] = col[i];
  }
}

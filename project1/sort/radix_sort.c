#include "sort.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

#define DIGIT_BASE 10

static inline int extract_digit(int value, int exponent) { return (value / exponent) % DIGIT_BASE; }

static void count_digits(int *arr, size_t size, int *count, int exponent) {
  for (size_t i = 0; i < size; i++) {
    int digit = extract_digit(*(arr + i), exponent);
    (*(count + digit))++;
  }
}

static void accumulate_digit_counts(int *count, int ascending) {
  if (ascending) {
    for (int i = 1; i < DIGIT_BASE; i++) {
      *(count + i) += *(count + i - 1);
    }
  } else {
    for (int i = DIGIT_BASE - 2; i >= 0; i--) {
      *(count + i) += *(count + i + 1);
    }
  }
}

static void sort_by_digit(int *arr, size_t size, int exponent, int ascending) {
  int *output = malloc(size * sizeof(int));
  int count[DIGIT_BASE] = {0};
  if (!output)
    return;

  count_digits(arr, size, count, exponent);
  accumulate_digit_counts(count, ascending);

  for (int i = size - 1; i >= 0; i--) {
    int value = *(arr + i);
    int digit = extract_digit(value, exponent);
    int output_pos = *(count + digit) - 1;
    *(output + output_pos) = value;
    (*(count + digit))--;
  }

  memcpy(arr, output, size * sizeof(int));
  free(output);
}

static int find_max_value(int *arr, size_t size) {
  int max_val = *arr;
  for (size_t i = 1; i < size; i++) {
    if (*(arr + i) > max_val) {
      max_val = *(arr + i);
    }
  }
  return max_val;
}

int *radix_sort_by_value(const int *arr, size_t size, int ascending) {
  int *copy = duplicate_vec1(arr, size);
  if (!copy) {
    return NULL;
  }
  radix_sort_by_pointer(copy, size, ascending);
  return copy;
}

void radix_sort_by_pointer(int *arr, size_t size, int ascending) {
  if (size == 0)
    return;

  int max_val = find_max_value(arr, size);

  for (int exponent = 1; max_val / exponent > 0; exponent *= DIGIT_BASE) {
    sort_by_digit(arr, size, exponent, ascending);
  }
}

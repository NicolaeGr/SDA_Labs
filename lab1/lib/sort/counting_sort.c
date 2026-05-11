#include "sort.h"
#include <stdlib.h>
#include <string.h>

static void find_range_array(int *arr, size_t size, int *min_val, int *max_val) {
  *min_val = *max_val = arr[0];
  for (size_t i = 1; i < size; i++) {
    int value = arr[i];
    if (value < *min_val) {
      *min_val = value;
    }
    if (value > *max_val) {
      *max_val = value;
    }
  }
}

static void find_range_pointer(int *arr, size_t size, int *min_val, int *max_val) {
  *min_val = *max_val = *arr;
  for (size_t i = 1; i < size; i++) {
    int value = *(arr + i);
    if (value < *min_val) {
      *min_val = value;
    }
    if (value > *max_val) {
      *max_val = value;
    }
  }
}

static void build_count_array_array(int *arr, size_t size, int *count, int min_val) {
  for (size_t i = 0; i < size; i++) {
    int offset = arr[i] - min_val;
    count[offset]++;
  }
}

static void build_count_array_pointer(int *arr, size_t size, int *count, int min_val) {
  for (size_t i = 0; i < size; i++) {
    int offset = *(arr + i) - min_val;
    (*(count + offset))++;
  }
}

static void accumulate_counts(int *count, size_t range, int ascending) {
  if (ascending) {
    for (size_t i = 1; i < range; i++) {
      count[i] += count[i - 1];
    }
  } else {
    for (size_t i = range - 1; i > 0; i--) {
      count[i - 1] += count[i];
    }
  }
}

void counting_sort_array(int *arr, size_t size, int ascending) {
  if (size == 0) {
    return;
  }

  int min_val;
  int max_val;
  find_range_array(arr, size, &min_val, &max_val);

  size_t range = (size_t)(max_val - min_val + 1);
  int *count = calloc(range, sizeof(int));
  int *output = malloc(size * sizeof(int));
  if (!count || !output) {
    free(count);
    free(output);
    return;
  }

  build_count_array_array(arr, size, count, min_val);
  accumulate_counts(count, range, ascending);

  for (int i = (int)size - 1; i >= 0; i--) {
    int value = arr[i];
    int count_idx = value - min_val;
    int output_pos = count[count_idx] - 1;
    output[output_pos] = value;
    count[count_idx]--;
  }

  memcpy(arr, output, size * sizeof(int));
  free(count);
  free(output);
}

void counting_sort_pointer(int *arr, size_t size, int ascending) {
  if (size == 0) {
    return;
  }

  int min_val;
  int max_val;
  find_range_pointer(arr, size, &min_val, &max_val);

  size_t range = (size_t)(max_val - min_val + 1);
  int *count = calloc(range, sizeof(int));
  int *output = malloc(size * sizeof(int));
  if (!count || !output) {
    free(count);
    free(output);
    return;
  }

  build_count_array_pointer(arr, size, count, min_val);
  accumulate_counts(count, range, ascending);

  for (int i = (int)size - 1; i >= 0; i--) {
    int value = *(arr + i);
    int count_idx = value - min_val;
    int output_pos = *(count + count_idx) - 1;
    *(output + output_pos) = value;
    (*(count + count_idx))--;
  }

  memcpy(arr, output, size * sizeof(int));
  free(count);
  free(output);
}

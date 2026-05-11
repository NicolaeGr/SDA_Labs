#include "sort.h"
#include <stdlib.h>
#include <string.h>

static inline int should_take_left(int left_val, int right_val, int ascending) {
  return ascending ? left_val <= right_val : left_val >= right_val;
}

static void merge_array(int *arr, size_t left_start, size_t mid, size_t right_end, int ascending) {
  size_t left_size = mid - left_start + 1;
  size_t right_size = right_end - mid;

  int *left_temp = malloc(left_size * sizeof(int));
  int *right_temp = malloc(right_size * sizeof(int));
  if (!left_temp || !right_temp) {
    free(left_temp);
    free(right_temp);
    return;
  }

  memcpy(left_temp, &arr[left_start], left_size * sizeof(int));
  memcpy(right_temp, &arr[mid + 1], right_size * sizeof(int));

  size_t left_idx = 0;
  size_t right_idx = 0;
  size_t merge_idx = left_start;

  while (left_idx < left_size && right_idx < right_size) {
    if (should_take_left(left_temp[left_idx], right_temp[right_idx], ascending)) {
      arr[merge_idx++] = left_temp[left_idx++];
    } else {
      arr[merge_idx++] = right_temp[right_idx++];
    }
  }

  while (left_idx < left_size) {
    arr[merge_idx++] = left_temp[left_idx++];
  }
  while (right_idx < right_size) {
    arr[merge_idx++] = right_temp[right_idx++];
  }

  free(left_temp);
  free(right_temp);
}

static void merge_pointer(int *arr, size_t left_start, size_t mid, size_t right_end, int ascending) {
  size_t left_size = mid - left_start + 1;
  size_t right_size = right_end - mid;

  int *left_temp = malloc(left_size * sizeof(int));
  int *right_temp = malloc(right_size * sizeof(int));
  if (!left_temp || !right_temp) {
    free(left_temp);
    free(right_temp);
    return;
  }

  memcpy(left_temp, arr + left_start, left_size * sizeof(int));
  memcpy(right_temp, arr + mid + 1, right_size * sizeof(int));

  size_t left_idx = 0;
  size_t right_idx = 0;
  size_t merge_idx = left_start;

  while (left_idx < left_size && right_idx < right_size) {
    if (should_take_left(*(left_temp + left_idx), *(right_temp + right_idx), ascending)) {
      *(arr + merge_idx++) = *(left_temp + left_idx++);
    } else {
      *(arr + merge_idx++) = *(right_temp + right_idx++);
    }
  }

  while (left_idx < left_size) {
    *(arr + merge_idx++) = *(left_temp + left_idx++);
  }
  while (right_idx < right_size) {
    *(arr + merge_idx++) = *(right_temp + right_idx++);
  }

  free(left_temp);
  free(right_temp);
}

static void merge_sort_helper_array(int *arr, size_t left, size_t right, int ascending) {
  if (left < right) {
    size_t mid = left + (right - left) / 2;
    merge_sort_helper_array(arr, left, mid, ascending);
    merge_sort_helper_array(arr, mid + 1, right, ascending);
    merge_array(arr, left, mid, right, ascending);
  }
}

static void merge_sort_helper_pointer(int *arr, size_t left, size_t right, int ascending) {
  if (left < right) {
    size_t mid = left + (right - left) / 2;
    merge_sort_helper_pointer(arr, left, mid, ascending);
    merge_sort_helper_pointer(arr, mid + 1, right, ascending);
    merge_pointer(arr, left, mid, right, ascending);
  }
}

void merge_sort_array(int *arr, size_t size, int ascending) {
  if (size > 1) {
    merge_sort_helper_array(arr, 0, size - 1, ascending);
  }
}

void merge_sort_pointer(int *arr, size_t size, int ascending) {
  if (size > 1) {
    merge_sort_helper_pointer(arr, 0, size - 1, ascending);
  }
}

#ifndef SORT_H
#define SORT_H

#include <stddef.h>

int *bubble_sort_by_value(const int *arr, size_t size, int ascending);
int *heap_sort_by_value(const int *arr, size_t size, int ascending);
int *counting_sort_by_value(const int *arr, size_t size, int ascending);
int *radix_sort_by_value(const int *arr, size_t size, int ascending);
int *comb_sort_by_value(const int *arr, size_t size, int ascending);
int *merge_sort_by_value(const int *arr, size_t size, int ascending);
int *quick_sort_by_value(const int *arr, size_t size, int ascending);
int *shell_sort_by_value(const int *arr, size_t size, int ascending);
int *selection_sort_by_value(const int *arr, size_t size, int ascending);
int *insertion_sort_by_value(const int *arr, size_t size, int ascending);

void bubble_sort_by_pointer(int *arr, size_t size, int ascending);
void heap_sort_by_pointer(int *arr, size_t size, int ascending);
void counting_sort_by_pointer(int *arr, size_t size, int ascending);
void radix_sort_by_pointer(int *arr, size_t size, int ascending);
void comb_sort_by_pointer(int *arr, size_t size, int ascending);
void merge_sort_by_pointer(int *arr, size_t size, int ascending);
void quick_sort_by_pointer(int *arr, size_t size, int ascending);
void shell_sort_by_pointer(int *arr, size_t size, int ascending);
void selection_sort_by_pointer(int *arr, size_t size, int ascending);
void insertion_sort_by_pointer(int *arr, size_t size, int ascending);

#endif // SORT_H

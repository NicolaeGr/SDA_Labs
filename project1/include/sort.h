#ifndef SORT_H
#define SORT_H

#include <stddef.h>

void bubble_sort_array(int *arr, size_t size, int ascending);
void bubble_sort_pointer(int *arr, size_t size, int ascending);

void heap_sort_array(int *arr, size_t size, int ascending);
void heap_sort_pointer(int *arr, size_t size, int ascending);

void counting_sort_array(int *arr, size_t size, int ascending);
void counting_sort_pointer(int *arr, size_t size, int ascending);

void radix_sort_array(int *arr, size_t size, int ascending);
void radix_sort_pointer(int *arr, size_t size, int ascending);

void comb_sort_array(int *arr, size_t size, int ascending);
void comb_sort_pointer(int *arr, size_t size, int ascending);

void merge_sort_array(int *arr, size_t size, int ascending);
void merge_sort_pointer(int *arr, size_t size, int ascending);

void quick_sort_array(int *arr, size_t size, int ascending);
void quick_sort_pointer(int *arr, size_t size, int ascending);

void shell_sort_array(int *arr, size_t size, int ascending);
void shell_sort_pointer(int *arr, size_t size, int ascending);

void selection_sort_array(int *arr, size_t size, int ascending);
void selection_sort_pointer(int *arr, size_t size, int ascending);

void insertion_sort_array(int *arr, size_t size, int ascending);
void insertion_sort_pointer(int *arr, size_t size, int ascending);

#endif // SORT_H

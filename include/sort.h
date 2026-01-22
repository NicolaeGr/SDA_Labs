#ifndef GENERIC_SORT_H
#define GENERIC_SORT_H

#include <stddef.h>

/**
 * Generic Quick Sort
 * Sorts an array of any struct type using a custom comparison function
 * @param base Base pointer to the array
 * @param count Number of elements in the array
 * @param elem_size Size of each element in bytes (use sizeof(YourStruct))
 * @param compare Comparison function pointer that returns:
 *                < 0 if first argument is less than second
 *                = 0 if arguments are equal
 *                > 0 if first argument is greater than second
 */
void quick_sort(void *base, size_t count, size_t elem_size, int (*compare)(const void *, const void *));

/**
 * Generic Merge Sort
 * Sorts an array of any struct type using a custom comparison function
 * @param base Base pointer to the array
 * @param count Number of elements in the array
 * @param elem_size Size of each element in bytes (use sizeof(YourStruct))
 * @param compare Comparison function pointer that returns:
 *                < 0 if first argument is less than second
 *                = 0 if arguments are equal
 *                > 0 if first argument is greater than second
 */
void merge_sort(void *base, size_t count, size_t elem_size, int (*compare)(const void *, const void *));

#endif // GENERIC_SORT_H

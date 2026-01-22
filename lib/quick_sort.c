#include "sort.h"
#include <stdlib.h>
#include <string.h>

static inline void* get_element(void* base, size_t index, size_t elem_size) {
    return (char*)base + (index * elem_size);
}

static void swap_elements(void* a, void* b, size_t elem_size) {
    char temp[elem_size];
    memcpy(temp, a, elem_size);
    memcpy(a, b, elem_size);
    memcpy(b, temp, elem_size);
}

static int partition(void* base, int left, int right, size_t elem_size, 
                     int (*compare)(const void*, const void*)) {
    void* pivot = get_element(base, right, elem_size);
    int i = left - 1;
    
    for (int j = left; j < right; j++) {
        void* elem_j = get_element(base, j, elem_size);
        if (compare(elem_j, pivot) <= 0) {
            i++;
            void* elem_i = get_element(base, i, elem_size);
            swap_elements(elem_i, elem_j, elem_size);
        }
    }
    
    void* elem_i_plus_1 = get_element(base, i + 1, elem_size);
    swap_elements(elem_i_plus_1, pivot, elem_size);
    
    return i + 1;
}

static void quick_sort_recursive(void* base, int left, int right, size_t elem_size,
                                  int (*compare)(const void*, const void*)) {
    if (left < right) {
        int pivot_index = partition(base, left, right, elem_size, compare);
        quick_sort_recursive(base, left, pivot_index - 1, elem_size, compare);
        quick_sort_recursive(base, pivot_index + 1, right, elem_size, compare);
    }
}

void quick_sort(void* base, size_t count, size_t elem_size, 
                int (*compare)(const void*, const void*)) {
    if (base == NULL || count <= 1 || elem_size == 0 || compare == NULL) {
        return;
    }
    quick_sort_recursive(base, 0, count - 1, elem_size, compare);
}

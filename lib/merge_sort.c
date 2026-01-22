#include "sort.h"
#include <stdlib.h>
#include <string.h>

static inline void* get_element(void* base, size_t index, size_t elem_size) {
    return (char*)base + (index * elem_size);
}

static void merge(void* base, int left, int mid, int right, void* temp, 
                  size_t elem_size, int (*compare)(const void*, const void*)) {
    int i = left;
    int j = mid + 1;
    int k = left;
    
    while (i <= mid && j <= right) {
        void* elem_i = get_element(base, i, elem_size);
        void* elem_j = get_element(base, j, elem_size);
        void* temp_k = get_element(temp, k, elem_size);
        
        if (compare(elem_i, elem_j) <= 0) {
            memcpy(temp_k, elem_i, elem_size);
            i++;
        } else {
            memcpy(temp_k, elem_j, elem_size);
            j++;
        }
        k++;
    }
    
    while (i <= mid) {
        void* elem_i = get_element(base, i, elem_size);
        void* temp_k = get_element(temp, k, elem_size);
        memcpy(temp_k, elem_i, elem_size);
        i++;
        k++;
    }
    
    while (j <= right) {
        void* elem_j = get_element(base, j, elem_size);
        void* temp_k = get_element(temp, k, elem_size);
        memcpy(temp_k, elem_j, elem_size);
        j++;
        k++;
    }
    
    for (i = left; i <= right; i++) {
        void* elem_i = get_element(base, i, elem_size);
        void* temp_i = get_element(temp, i, elem_size);
        memcpy(elem_i, temp_i, elem_size);
    }
}

static void merge_sort_recursive(void* base, int left, int right, void* temp,
                                  size_t elem_size, int (*compare)(const void*, const void*)) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        merge_sort_recursive(base, left, mid, temp, elem_size, compare);
        merge_sort_recursive(base, mid + 1, right, temp, elem_size, compare);
        merge(base, left, mid, right, temp, elem_size, compare);
    }
}

void merge_sort(void* base, size_t count, size_t elem_size,
                int (*compare)(const void*, const void*)) {
    if (base == NULL || count <= 1 || elem_size == 0 || compare == NULL) {
        return;
    }
    
    void* temp = malloc(count * elem_size);
    if (temp == NULL) {
        return;
    }
    
    merge_sort_recursive(base, 0, count - 1, temp, elem_size, compare);
    
    free(temp);
}

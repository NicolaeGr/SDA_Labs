#ifndef ARRAY_UTILS_H
#define ARRAY_UTILS_H

#include <stddef.h>

int is_prime(int n);

void populate_vec1(int *arr, size_t n, int min, int max);
void populate_vec2(int *arr, size_t rows, size_t cols, int min, int max);
int *create_and_populate_vec1(size_t n, int min, int max);
int *create_and_populate_vec2(size_t rows, size_t cols, int min, int max);

double calculate_avg_at_even_positions(const int *arr, size_t size);
double calculate_avg_at_odd_positions(const int *arr, size_t size);
int has_prime_numbers(const int *arr, size_t size);
int calculate_product_of_negatives(const int *arr, size_t size, int *product);

int count_above_main_diagonal(size_t n);
void extract_secondary_diagonal(const int *arr, size_t n, int *diag);
void place_secondary_diagonal(int *arr, size_t n, const int *diag);
void extract_first_column(const int *arr, size_t rows, size_t cols, int *col);
void place_first_column(int *arr, size_t rows, size_t cols, const int *col);
void find_max_info(const int *arr, size_t rows, size_t cols, int *max_val, int *count, int **positions);
void extract_row(const int *arr, size_t cols, size_t row_idx, int *row);
void place_row(int *arr, size_t cols, size_t row_idx, const int *row);
void extract_column(const int *arr, size_t rows, size_t cols, size_t col_idx, int *col);
void place_column(int *arr, size_t rows, size_t cols, size_t col_idx, const int *col);

#endif // ARRAY_UTILS_H

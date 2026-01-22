#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

void setup_terminal_cleanup(void);
void enter_alternate_screen(void);
void exit_alternate_screen(void);
void clear_screen(void);
void print_separator(char ch, int length);
void clear_input_buffer(void);

void wait_for_enter(void);

int get_int_input(const char *prompt);
int get_int_input_range(const char *prompt, int min, int max);
double get_double_input(const char *prompt);
double get_double_input_range(const char *prompt, double min, double max);
int get_yes_no_input(const char *prompt, int default_yes);

void print_vec1(const int *arr, size_t size, const char *title);
void print_vec2(const int *arr, size_t rows, size_t cols, const char *title);

void input_vec1(int *arr, size_t size);
void input_vec2(int *arr, size_t rows, size_t cols);

void generate_random_vec1(int *arr, size_t size, int min, int max);
void generate_random_vec2(int *arr, size_t rows, size_t cols, int min, int max);

void copy_vec1(int *dest, const int *src, size_t size);
void copy_vec2(int *dest, const int *src, size_t rows, size_t cols);

int *duplicate_vec1(const int *src, size_t size);
int *duplicate_vec2(const int *src, size_t rows, size_t cols);

#endif // UTILS_H

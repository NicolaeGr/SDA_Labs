#ifndef LAB5_STACK_QUEUE_H
#define LAB5_STACK_QUEUE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "common/product/product.h"

typedef struct StackNode {
  Product data;
  struct StackNode *next;
} StackNode;

typedef struct {
  StackNode *top;
  size_t size;
} ProductStack;

typedef struct QueueNode {
  Product data;
  struct QueueNode *next;
} QueueNode;

typedef struct {
  QueueNode *front;
  QueueNode *rear;
  size_t size;
} ProductQueue;

void stack_init(ProductStack *stack);
void stack_free(ProductStack *stack);
bool stack_push(ProductStack *stack, const Product *item);
size_t stack_size(const ProductStack *stack);
Product *stack_get_at(ProductStack *stack, size_t index_from_top);
const Product *stack_find_max_price(const ProductStack *stack);
bool stack_write_text(const ProductStack *stack, FILE *file);
bool stack_write_binary(const ProductStack *stack, FILE *file);
bool stack_copy_to_queue(const ProductStack *stack, ProductQueue *queue);

void queue_init(ProductQueue *queue);
void queue_free(ProductQueue *queue);
bool queue_enqueue(ProductQueue *queue, const Product *item);
bool queue_write_text(const ProductQueue *queue, FILE *file);
bool queue_write_binary(const ProductQueue *queue, FILE *file);

#endif

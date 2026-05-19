#include "stack_queue.h"

#include <stdlib.h>
#include <string.h>

void stack_init(ProductStack *stack) {
  if (!stack) {
    return;
  }

  stack->top = NULL;
  stack->size = 0;
}

void stack_free(ProductStack *stack) {
  if (!stack) {
    return;
  }

  StackNode *node = stack->top;
  while (node) {
    StackNode *next = node->next;
    free(node);
    node = next;
  }

  stack->top = NULL;
  stack->size = 0;
}

bool stack_push(ProductStack *stack, const Product *item) {
  if (!stack || !item) {
    return false;
  }

  StackNode *node = (StackNode *)malloc(sizeof(StackNode));
  if (!node) {
    return false;
  }

  node->data = *item;
  node->next = stack->top;
  stack->top = node;
  stack->size++;
  return true;
}

size_t stack_size(const ProductStack *stack) { return stack ? stack->size : 0; }

Product *stack_get_at(ProductStack *stack, size_t index_from_top) {
  if (!stack || index_from_top >= stack->size) {
    return NULL;
  }

  StackNode *node = stack->top;
  for (size_t i = 0; i < index_from_top; i++) {
    node = node->next;
  }

  return node ? &node->data : NULL;
}

const Product *stack_find_max_price(const ProductStack *stack) {
  if (!stack || !stack->top) {
    return NULL;
  }

  const StackNode *node = stack->top;
  const Product *max_item = &node->data;
  node = node->next;

  while (node) {
    if (node->data.price > max_item->price) {
      max_item = &node->data;
    }
    node = node->next;
  }

  return max_item;
}

bool stack_write_text(const ProductStack *stack, FILE *file) {
  if (!stack || !file) {
    return false;
  }

  fprintf(file, "STACK (top -> bottom)\n");
  fprintf(file, "============================================================\n");

  const StackNode *node = stack->top;
  size_t idx = 1;
  while (node) {
    const Product *p = &node->data;
    fprintf(file, "%zu) %s | %s | %s | %s | %.2f\n", idx, p->name, p->country, p->manufacturer, p->item_code, p->price);
    node = node->next;
    idx++;
  }

  fprintf(file, "Total: %zu\n", stack->size);
  return true;
}

bool stack_write_binary(const ProductStack *stack, FILE *file) {
  if (!stack || !file) {
    return false;
  }

  if (fwrite(&stack->size, sizeof(stack->size), 1, file) != 1) {
    return false;
  }

  const StackNode *node = stack->top;
  while (node) {
    if (fwrite(&node->data, sizeof(Product), 1, file) != 1) {
      return false;
    }
    node = node->next;
  }

  return true;
}

void queue_init(ProductQueue *queue) {
  if (!queue) {
    return;
  }

  queue->front = NULL;
  queue->rear = NULL;
  queue->size = 0;
}

void queue_free(ProductQueue *queue) {
  if (!queue) {
    return;
  }

  QueueNode *node = queue->front;
  while (node) {
    QueueNode *next = node->next;
    free(node);
    node = next;
  }

  queue->front = NULL;
  queue->rear = NULL;
  queue->size = 0;
}

bool queue_enqueue(ProductQueue *queue, const Product *item) {
  if (!queue || !item) {
    return false;
  }

  QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
  if (!node) {
    return false;
  }

  node->data = *item;
  node->next = NULL;

  if (!queue->rear) {
    queue->front = node;
    queue->rear = node;
  } else {
    queue->rear->next = node;
    queue->rear = node;
  }

  queue->size++;
  return true;
}

bool stack_copy_to_queue(const ProductStack *stack, ProductQueue *queue) {
  if (!stack || !queue) {
    return false;
  }

  queue_free(queue);
  queue_init(queue);

  // Preserve stack top->bottom order in queue front->rear.
  const StackNode *node = stack->top;
  while (node) {
    if (!queue_enqueue(queue, &node->data)) {
      return false;
    }
    node = node->next;
  }

  return true;
}

bool queue_write_text(const ProductQueue *queue, FILE *file) {
  if (!queue || !file) {
    return false;
  }

  fprintf(file, "QUEUE (front -> rear)\n");
  fprintf(file, "============================================================\n");

  const QueueNode *node = queue->front;
  size_t idx = 1;
  while (node) {
    const Product *p = &node->data;
    fprintf(file, "%zu) %s | %s | %s | %s | %.2f\n", idx, p->name, p->country, p->manufacturer, p->item_code, p->price);
    node = node->next;
    idx++;
  }

  fprintf(file, "Total: %zu\n", queue->size);
  return true;
}

bool queue_write_binary(const ProductQueue *queue, FILE *file) {
  if (!queue || !file) {
    return false;
  }

  if (fwrite(&queue->size, sizeof(queue->size), 1, file) != 1) {
    return false;
  }

  const QueueNode *node = queue->front;
  while (node) {
    if (fwrite(&node->data, sizeof(Product), 1, file) != 1) {
      return false;
    }
    node = node->next;
  }

  return true;
}

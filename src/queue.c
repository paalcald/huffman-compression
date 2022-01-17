#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

queue_t *q_create() {
  queue_t *newqueue = (queue_t *) malloc(sizeof(queue_t));
  newqueue->queue = (QUEUE_TYPE *) malloc(QUEUE_INIT_CAP * sizeof(QUEUE_TYPE));
  newqueue->top =  0;
  newqueue->size = 0;
  newqueue->capacity = QUEUE_INIT_CAP;
  return newqueue;
}

void q_enqueue(QUEUE_TYPE item, queue_t *q ) {
  if (!q_full(q)) {
    q->queue[(q->top + q->size) % q->capacity] = item;
    q->size++;
  } else {
    QUEUE_TYPE* newqueue = (QUEUE_TYPE*) malloc (q->capacity * 2 * sizeof(QUEUE_TYPE));
    for (int i = 0; i < q->capacity; i++) {
      newqueue[i] = q->queue[(q->top + i) % q->capacity];
    }
    free(q->queue);
    q->queue = newqueue;
    q->queue[q->size] = item;
    q->top = 0;
    q->size++;
    q->capacity *= 2;
  }
}
 
QUEUE_TYPE q_dequeue(queue_t *q) {
  QUEUE_TYPE result = q->queue[q->top];
  
  if (q->top < q->capacity - 1)
    q->top++;
  else
    q->top = 0;

  q->size--;
  return result;
}

void q_print(queue_t *q) {
  if (q->size != 0) {
    printf("[ ");
    for (int i = 0; i <= q->size; i++) {
      if (q->queue[(q->top + i) % q->capacity] == 0)
	putchar('0');
      if (q->queue[(q->top + i) % q->capacity] == 1)
	putchar('1');
      printf(", ");
    }
    printf("\033[2D]\n");
  }
}

bool q_empty(queue_t *q) {
  return (q->size == 0);
}

bool q_full(queue_t *q) {
  return (q->size == q->capacity);
}

void q_free(queue_t *q) {
  free(q->queue);
  free(q);
}

void q_enqueue_char(char c, queue_t *q)
{
    for (int i = 7; i >= 0; --i)
    {
      if (c & (1 << i)) {
	q_enqueue(1, q);
      } else {
	q_enqueue(0, q);
      }
    }
}

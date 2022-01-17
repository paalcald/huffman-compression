#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

#define QUEUE_TYPE bool
#define QUEUE_INIT_CAP 4

typedef struct _queue {
  QUEUE_TYPE *queue;
  int top;
  int size;
  int capacity;
} queue_t;

queue_t *q_create();
void q_enqueue(QUEUE_TYPE item, queue_t *q );
QUEUE_TYPE q_dequeue(queue_t *q);
bool q_empty(queue_t *q);
bool q_full(queue_t *q);
void q_print(queue_t *q);
void q_free(queue_t *q);
void q_enqueue_char(char c, queue_t *q);

#endif //QUEUE_H

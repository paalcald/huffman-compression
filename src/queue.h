#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

#define QUEUE_TYPE bool

typedef struct _node {
  QUEUE_TYPE value;
  struct _node* next;
} node_t;

typedef struct _queue {
  node_t * head;
  node_t * tail;
  int size;
} queue_t;

queue_t *q_create();
void q_enqueue(QUEUE_TYPE item, queue_t *q );
QUEUE_TYPE q_dequeue(queue_t *q);
void q_print(queue_t *q);
void q_free(queue_t *q);
void q_enqueue_char(char c, queue_t *q);

#endif //QUEUE_H

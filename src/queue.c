#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

queue_t *q_create() {
  queue_t *newqueue = (queue_t *) malloc(sizeof(queue_t));
  newqueue->head = NULL;
  newqueue->tail = NULL;
  newqueue->size = 0;
  return newqueue;
}

void q_enqueue(QUEUE_TYPE item, queue_t *q ) {
  node_t *newnode = (node_t *) malloc (sizeof(node_t));
  newnode->value = item;
  newnode->next = NULL;
  if (q->tail != NULL) {
    q->tail->next = newnode;
  }
  q->tail = newnode;
  q->size++;
  if(q->head == NULL) {
    q->head = newnode;
  }
}

QUEUE_TYPE q_dequeue(queue_t *q) {
  node_t *tmp = q->head;
  int result = tmp->value;
  q->head = q->head->next;
  q->size--;
  if (q->head == NULL) {
    q->tail = NULL;
  }
  free(tmp);
  return result;
}

void q_print(queue_t *q) {
  if (q->size != 0) {
    node_t *n = q->head;
    printf("[ ");
    for (int i = 0; i <= q->size; i++) {
      if (n->value == 0)
	putchar('0');
      if (n->value == 1)
	putchar('1');
      printf(", ");
      n = n->next;
    }
    printf("\033[2D]\n");
  }
}

void q_free(queue_t *q) {
  while(q->size != 0) {
    q_dequeue(q);
  }
  q_print(q);
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

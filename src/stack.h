#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#define STACK_TYPE char

typedef struct _snode {
  STACK_TYPE value;
  struct _snode *next;
} snode_t;

typedef snode_t * stack_t;

void s_push(STACK_TYPE elem, stack_t *s);
STACK_TYPE s_pop(stack_t *s);
bool s_empty(stack_t s);
void s_free(stack_t *s);

#endif //STACK_H

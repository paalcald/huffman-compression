#include "stack.h"
#include <stdlib.h>

void s_push(STACK_TYPE elem, stack_t *s) {
  
  snode_t *newnode = (snode_t*) malloc(sizeof(snode_t));
  newnode->value = elem;
  newnode->next = *s;
  (*s) = newnode;
}

STACK_TYPE s_pop(stack_t *s) {
  snode_t *tmp = *s;
  *s = (*s)->next;
  STACK_TYPE result = tmp->value;
  free(tmp);
  return(result);
}

bool s_empty(stack_t s) {
  return (s == NULL);
}

void s_free(stack_t *s) {
  while(!s_empty(*s)) {
    s_pop(s);
  }
}

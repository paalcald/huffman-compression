#include <stdlib.h>
#include <stdio.h>
#include "huffman_tree.h"
#include "stack.h"

hnode_t *hn_create_leaf(char c, int f)
{
  hnode_t *newnode = (hnode_t *) malloc(sizeof (hnode_t));
  newnode->type = LEAF;
  newnode->frequency = f;
  newnode->leaf.character = c;

  return newnode;
}

hnode_t *hn_create_node(hnode_t *n1, hnode_t *n2)
{
  hnode_t *newnode = (hnode_t *) malloc(sizeof(hnode_t));
  newnode->type = NODE;
  newnode->frequency = n1->frequency + n2->frequency;
  newnode->node.left_tree = n1;
  newnode->node.right_tree = n2;

  return newnode;
}

void ht_print (hnode_t *root)
{
  if (root->type==LEAF) {
    printf("(%i, %i)", root->leaf.character, root->frequency);
  } else {
    printf("[%i ", root->frequency);
    ht_print(root->node.left_tree);
    printf(" ");
    ht_print(root->node.right_tree);
    printf("]");
  }
}

void ht_insert(hnode_t** root, char c, int f)
{
  if (*root == NULL) {
    *root = hn_create_leaf(c, f);
  }
}

int ht_getfreq(hnode_t* root) {
  return root->frequency;
}

bool ht_isleaf(hnode_t* root) {
  return (root->type == LEAF);
}

char ht_getchar(hnode_t *root) {
  return (root->leaf.character);
}

void _ht_preorder_r(hnode_t *root, char *str, int *i, char c) {
  if (ht_isleaf(root)) {
    str[*i] = ht_getchar(root);
    (*i)++;
  } else {
    str[*i] = c;
    (*i)++;
    _ht_preorder_r(root->node.left_tree, str, i, c);
    _ht_preorder_r(root->node.right_tree, str, i, c);
  }  
}

char* ht_getstr(hnode_t *root, char c) {
  char *str = (char *) malloc(512);
  int i = 0;
  _ht_preorder_r(root, str, &i, c);
  str[i] = '\0';
  return str;
}

void ht_free(hnode_t *root) {
  if (ht_isleaf(root)) {
    free(root);
  } else {
    ht_free(root->node.left_tree);
    ht_free(root->node.right_tree);
    free(root);
  }
}

char *hstr_next(char* c) {
  return c + 1;
}

char *hstr_left(char* node) {
  return hstr_next(node);
}

char *hstr_right(char* node) {
  char *n = node;
  stack_t s = NULL;
  s_push(*n, &s);
  n = hstr_next(n);
  while (!s_empty(s)) {
    if (*n == HUFF_STR_SEP)
      s_push(*n, &s);
    else
      s_pop(&s);
    n = hstr_next(n);
  }
  return n;
}

void _hstrtohtree(char **str, hnode_t *root) {
  char c = **str;
  (*str)++;
  if (**str == HUFF_STR_SEP) {
    root->type = NODE;
    hnode_t *left_node = (hnode_t*) malloc(sizeof(hnode_t));
    hnode_t *right_node = (hnode_t*) malloc(sizeof(hnode_t));
    root->node.left_tree = left_node;
    root->node.right_tree = right_node;
    _hstrtohtree(str, left_node);
    _hstrtohtree(str, right_node);
  } else {
    root->type = LEAF;
    root->leaf.character = c;
  }
}

hnode_t *hstrtohtree(char *str) {
  hnode_t *node = (hnode_t *) malloc(sizeof(hnode_t));
  _hstrtohtree(&str, node);
  
  return node;
}

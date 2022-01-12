#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <stdbool.h>

#define HUFF_STR_SEP (char) 27

typedef enum { NODE, LEAF } node_type;

struct _hnode;

typedef struct _hleaf {
  char character;
} hleaf_t;

typedef struct _hint_node {
  struct _hnode* left_tree;
  struct _hnode* right_tree;
} hint_node_t;

typedef struct _hnode {
  node_type type;
  int frequency;
  union {
    struct _hleaf leaf;
    struct _hint_node node;
  };
} hnode_t;

hnode_t *hn_create_leaf(char c, int f);
hnode_t *hn_create_node(hnode_t *n1, hnode_t *n2);
void ht_print(hnode_t *root);
void ht_insert(hnode_t **root, char c, int f);
int ht_getfreq(hnode_t *root);
bool ht_isleaf(hnode_t *root);
char ht_getchar(hnode_t *root);
void _ht_preorder_r(hnode_t *root, char *str, int *i, char c);
char* ht_getstr(hnode_t *root, char c);
void ht_free(hnode_t *root);
char *hstr_next(char* c);
char *hstr_left(char* node);
char *hstr_right(char* node);

#endif //HUFFMAN_TREE_H

#ifndef MINHEAP_H
#define MINHEAP_H

#include "huffman_tree.h"

#define MINHEAP_TYPE hnode_t
#define MINHEAP_MAX_SIZE 256
#define MINHEAP_TYPE_GETTER(elem) (ht_getfreq(elem))

typedef struct __minheap {
	MINHEAP_TYPE* tree[MINHEAP_MAX_SIZE];
	int size;
} minheap_t;


int mh_parent(int i);
int mh_left(int i);
int mh_right(int i);
int mh_getfreq(int i ,const minheap_t *heap);
int mh_getsize(const minheap_t *heap);
MINHEAP_TYPE *mh_getmin(const minheap_t *heap);
minheap_t *mh_create();
void mh_swap(int i, int j, minheap_t *heap);
void mh_heapify(int i, minheap_t *heap);
MINHEAP_TYPE *mh_extractmin(minheap_t *heap);
void mh_insert(MINHEAP_TYPE* elem, minheap_t *heap);
void mh_print(minheap_t *heap);
void mh_free(minheap_t *heap);

#endif //MINHEAP_H

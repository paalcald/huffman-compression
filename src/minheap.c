#include <stdio.h>
#include <stdlib.h>
#include "huffman_tree.h"
#include "minheap.h"

int mh_parent(int i) {
	return (i - 1) / 2;
}

int mh_left(int i) {
	return 2*i + 1;
}

int mh_right(int i) {
	return 2*i + 2;
}

int mh_getfreq(int i ,const minheap_t *heap) {
	return MINHEAP_TYPE_GETTER(heap->tree[i]);
}

int mh_getsize(const minheap_t *heap) {
	return heap->size;
}

MINHEAP_TYPE *mh_getmin(const minheap_t *heap) {
	return heap->tree[heap->size - 1];
}

minheap_t *mh_create() {
	minheap_t *newheap = (minheap_t *) malloc(sizeof(minheap_t));
	newheap->size = 0;
	return newheap;
}

void mh_swap(int i, int j, minheap_t *heap) {
	MINHEAP_TYPE* tmp = heap->tree[i];
	heap->tree[i] = heap->tree[j];
	heap->tree[j] = tmp;
}

void mh_heapify(int i, minheap_t *heap) {
	int l = mh_left(i);
	int r = mh_right(i);
	int smallest = i;
	
	if (l < heap->size && mh_getfreq(l, heap) < mh_getfreq(r, heap))
		smallest = l;
	if (r < heap->size && mh_getfreq(r, heap) < mh_getfreq(smallest, heap))
		smallest = r;
	if (smallest != i) {
		mh_swap(i, smallest, heap);
		mh_heapify(smallest, heap);
	}
}

MINHEAP_TYPE *mh_extractmin(minheap_t *heap) {
  
	MINHEAP_TYPE *root = heap->tree[0];
	heap->tree[0] = heap->tree[--heap->size];
	
	//ht_free(heap->tree[heap->size]);
	mh_heapify(0, heap);
	
	return root;
}

void mh_insert(MINHEAP_TYPE* elem, minheap_t *heap) {
	int i = heap->size++;
	heap->tree[i] = elem;
	while ( i != 0 && mh_getfreq(mh_parent(i), heap) > mh_getfreq(i, heap)) {
		mh_swap(mh_parent(i), i, heap);
		i = mh_parent(i);
	}
}

void mh_print(minheap_t *heap) {
	for (int i = 0; i < mh_getsize(heap); i++) {
		ht_print(heap->tree[i]);
		printf("\n");
	}
}

void mh_free(minheap_t *heap) {
	for (int i = 0; i < MINHEAP_MAX_SIZE; i++) {
		ht_free(heap->tree[i]);
	}
}

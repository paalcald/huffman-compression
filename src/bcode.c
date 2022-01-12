#define CODE_MAX_SIZE 256

#include <stdio.h>
#include <stdlib.h>
#include "bcode.h"

bcode_t *bc_create(bool* code, int size) {
  bcode_t *newcode = (bcode_t*) malloc(sizeof(bcode_t));
  for (int i = 0; i < size; i++) {
    newcode->code[i] = code[i];
  }
  newcode->size = size;
  return newcode;
}
void bc_free(bcode_t* bcode) {
  free(bcode->code);
}

void bc_print(bcode_t *bcode) {
  if (bcode != NULL) { 
    for (int i = 0; i < bcode->size; i++) {
      if (bcode->code[i] == 0)
	printf("0");
      else
	printf("1");
    }
  }
}

void bc_arr_print(bcode_t** arr, int len) {
  for (int i = 0; i < len; i++) {
    printf("%i : ", i);
    bc_print(arr[i]);
    printf("\n");
  }
}

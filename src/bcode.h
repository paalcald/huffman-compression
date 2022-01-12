#ifndef BCODE_H
#define BCODE_H

#include <stdbool.h>

#define CODE_MAX_SIZE 256


typedef struct _binarycode {
  bool code[CODE_MAX_SIZE];
  int size;
} bcode_t;

bcode_t *bc_create(bool* code, int size);
void bc_free(bcode_t* bcode);
void bc_print(bcode_t *bcode);
void bc_arr_print(bcode_t** arr, int len);

#endif //BCODE_H

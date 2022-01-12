#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include "huffman_tree.h"
#include "minheap.h"
#include "stack.h"
#include "queue.h"
#include "bcode.h"

#define CHAR_MAX_VAL 256
#define HUFF_EXT ".huff"
#define HUFF_EXT_LEN 5
#define HUFF_STR_SEP (char) 27

char *straddhuff(char *filename) {
  size_t len = strlen(filename);
  char *extension = HUFF_EXT;
  size_t huff_extension_len = strlen(extension);
 
  char *huffed_filename = (char *) malloc((len + huff_extension_len + 1));

  for (size_t i = 0; i < len; i++) {
    huffed_filename[i] = filename[i];
  }
  for (size_t i = 0; i < huff_extension_len; i++) {
    huffed_filename[len + i] = extension[i];
  }
  huffed_filename[len + huff_extension_len] = '\0';
  
  return huffed_filename;
}

char *strrmhuff(char *filename) {
  size_t len = strlen(filename) - HUFF_EXT_LEN;
  char* trimmed = (char *) malloc (len + 1);

  for (size_t i = 0; i < len; i++) {
    trimmed[i] = filename[i];
  }
  trimmed[len] = '\0';
  return trimmed;
}

bool strcheckhuff(char *filename) {
  size_t len = strlen(filename);
  char *extension = HUFF_EXT;
  size_t trimmed_len = len - HUFF_EXT_LEN;
  bool check = true;
  for (size_t i = 0; i < HUFF_EXT_LEN; i++) {
    if (filename[trimmed_len + i] != extension[i])
      check = false;
  }
  return check;
}
    

unsigned long fgetcfm(FILE *file, int *freq) {
  unsigned long size = 0;
  int c;
  
  while ((c = fgetc(file)) != EOF) {
    freq[(unsigned char) c]++;
    size++;
  }
  
  return size;
}

minheap_t *cfmtomh(int * fmap) {
  minheap_t *mh = mh_create();
  for (int i = 0; i < CHAR_MAX_VAL; i++) {
    if (fmap[i] > 0) {
      mh_insert(hn_create_leaf(i, fmap[i]), mh);
    }
  }
  return mh;
}

hnode_t *mhtoht(minheap_t *mh) {
  hnode_t *a;
  hnode_t *b;
  
  while (mh_getsize(mh) >= 2) {
    a = mh_extractmin(mh);
    b = mh_extractmin(mh);
    mh_insert(hn_create_node(a, b), mh);
  }

  return mh_extractmin(mh);
}

void cm_free(bcode_t** charmap) {
  for (int i = 0; i < CODE_MAX_SIZE; i++) {
    bc_free(charmap[i]);
  }
  free(charmap);
}

void _hsubttocm(hnode_t *root, bcode_t** charmap, bool* prefix, int prefixsize) {
  if (ht_isleaf(root)) {
    charmap[(unsigned char) ht_getchar(root)] = bc_create(prefix, prefixsize);
  }
  if (!ht_isleaf(root)) {
    prefix[prefixsize] = 0;
    _hsubttocm(root->node.left_tree, charmap, prefix, prefixsize + 1);
    prefix[prefixsize] = 1;
    _hsubttocm(root->node.right_tree, charmap, prefix, prefixsize + 1);
  }
}

bcode_t **httocm(hnode_t* root) {
  bcode_t** charmap = (bcode_t**) calloc(CHAR_MAX_VAL, sizeof(bcode_t*));
  bool* prefix = (bool*) calloc(CHAR_MAX_VAL, sizeof(bool));
  _hsubttocm(root, charmap, prefix, 0);
  free(prefix);
  return charmap;
}

char eightbtochar(bool* bits) {
  unsigned char c = 0;
  for (int i = 0; i < 8; i++) {
    c |= bits[i] << (7-i);
  }
  return c;
}


void encode(FILE* input_file, unsigned long input_file_size, FILE* output_file, bcode_t **charmap, char* hstr) {
  // null terminated tree lenght in string format 
  size_t hstr_size = strlen(hstr);
  char str_tsize[16];
  
  sprintf(str_tsize, "%lu", hstr_size);
  for (size_t i = 0; i < strlen(str_tsize); i++) {
    fputc(str_tsize[i], output_file);
  }
  fputc('\0', output_file);
  
  //tree in string format
  for (size_t i = 0; i < hstr_size; i++) {
    fputc(hstr[i], output_file);
  }
  //null terminated file size in string format
  char str_isize[16];
  
  sprintf(str_isize, "%lu", input_file_size);
  for (size_t i = 0; i < strlen(str_isize); i++) {
    fputc(str_isize[i], output_file);
  }
  fputc('\0', output_file);
  
  //file encoded
  bcode_t *bcode;
  bool bit[8];
  int nbits = 0;
  int j = 0;
  int c, w;
  
  while ((c = fgetc(input_file)) != EOF) {
    bcode = charmap[(unsigned char) c];
    nbits = bcode->size;
    for (int i = 0; i < nbits; i++) {
      if (j == 8) {
	w = eightbtochar(bit);
	fputc(w, output_file);
	j = 0;
      }
      bit[j++] = bcode->code[i];
    }
  }
  while (j < 8) {
    bit[j++] = 0;
  }
  w = eightbtochar(bit);
  fputc(w, output_file);
}

void decode(FILE* input_file, FILE* output_file) {
  int c;
  //read null terminated tree lenght in string form
  char str_tree_size[16];
  unsigned long tree_size;
  int i = 0;
  while((c = fgetc(input_file)) != '\0') {
    str_tree_size[i] = c;
    i++;
  }
  str_tree_size[i] = '\0';
  tree_size = strtoul(str_tree_size, NULL, 10);
  //read tree in string form
  char root[tree_size + 1];
  for(size_t j = 0; j < tree_size; j++) {
    c = fgetc(input_file);
    root[j] = c;
  }
  root[tree_size] = '\0';
  //read null terminated file size in string form
  char str_file_size[16];
  unsigned long file_size;
  i = 0;
  while((c = fgetc(input_file)) != '\0') {
    str_file_size[i] = c;
    i++;
  }
  str_file_size[i] = '\0';
  file_size = strtoul(str_file_size, NULL, 10);
  
  //decode file
  char w;
  bool bit;
  char *n = root;
  queue_t *q = q_create();

  while((c = fgetc(input_file)) != EOF) {
    q_enqueue_char(c, q);
    while (q->size > 0) {
      bit = q_dequeue(q);
      if (bit == 0) {
	n = hstr_left(n);
      }
      if (bit == 1) {
	n = hstr_right(n);
      }
      if (*n != HUFF_STR_SEP) {
	w = *n;
	if (file_size == 0) {
	  q_free(q);
	  return;
	}
	fputc(w, output_file);
	n = root;
	file_size--;
      }
    }
  }
  q_free(q);
}

void compress(char* filename) {
  
  FILE *inputfile, *outputfile;
  //open input file
  if ((inputfile = fopen(filename, "r")) == NULL) {
    printf("%s isn't a valid filename", filename);
    printf("Unvalid file.\n");
    return;
  }

  // obtain file size and fill minheap with frequency nodes
  int *f = (int *) calloc(CHAR_MAX_VAL, sizeof(int));
  unsigned long file_size = fgetcfm(inputfile, f);
  minheap_t *minheap = cfmtomh(f);
  free(f);

  //get huffman tree from the minheap
  hnode_t *htree = mhtoht(minheap);

  //get codes and string form of huffman tree
  char *hstr = ht_getstr(htree, HUFF_STR_SEP);
  bcode_t** charmap = httocm(htree);
  ht_free(htree);

  //setup filenames and open output file
  fseek(inputfile, 0, SEEK_SET);
  char *compressed_filename = straddhuff(filename);
  if ((outputfile = fopen(compressed_filename, "w")) == NULL) {
      printf("Could not write the compressed file.\n");
      return;
  }
  
  //encode the file
  encode(inputfile, file_size, outputfile, charmap, hstr);
  
  //cleanup
  free(hstr);
  cm_free(charmap);
  fclose(inputfile);
  fclose(outputfile);
}

void decompress(char *filename) {

  //check that is the right extension
  if (!strcheckhuff(filename)) {
    printf("Unvalid file extension.\n");
    return;
  }
  //get filename
  char *outputfilename = strrmhuff(filename);
  //open input file and output file
  FILE *inputfile, *outputfile;
  if ((inputfile = fopen(filename, "r")) == NULL) {
    printf("Unvalid file.\n");
    return;
  }
  
  if ((outputfile = fopen(outputfilename, "w")) == NULL) {
    printf("Cannot open file to decompress.\n");
    return;
  }
  
  //decompress
  decode(inputfile, outputfile);
  
  //cleanup
  free(outputfilename);
  fclose(inputfile);
  fclose(outputfile);
}

void print_default_help() {
    printf("huff: You must specift one of the '-c' compress or '-x' extract options.\n");
    printf("Try 'huff -h' for more information.\n");
}

int readinput(char* input) {
  char *compress = "-c";
  char *extract = "-x";
  char *help = "-h";
  if (!strcmp(input, compress)) {
    return 1;
  }
  if (!strcmp(input, extract)) {
    return 2;
  }
  if (!strcmp(input, help)) {
    return 3;
  }
  else {
    return 0;
  }
}

int main(int argc, char** argv) {
  setlocale(LC_CTYPE, "");
  if (argc != 3) {
    print_default_help();
  }
  if (argc == 3) {
    int input = readinput(argv[1]);
    if (input == 0) {
      print_default_help();
    }
    if (input == 1) {
      printf("Compressing %s\n", argv[2]);
      compress(argv[2]);
    }
    if (input == 2) {
      printf("Extracting %s\n", argv[2]);
      decompress(argv[2]);
    }
    if (input == 3) {
      printf("huff: You must specift one of the '-c' compress or '-x' extract options.\n");
      printf("I didn't really do a documentation string for this you got me!\n");
    }
  }

}

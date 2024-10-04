#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sizes.h"

#define SEED 2216
#define SIZES 15
#define BUFFER_SIZE 256
#define MAX_VAL 256
#define HEADER_SIZE 4
#define FILENAME_LENGTH 256

int main(int argc, char **argv) {

  if (argc < 3 || argc > 4) {
    fprintf(stderr, "Usage: %s DATA_FILE_PATH SIZE [ident]", argv[0]);
    return 1;
  }

  size_t ident = 0;
  if (argc == 4) {
    ident = !strncmp(argv[3], "ident", 5);
  }

  srand(SEED);

  FILE *f;
  DATA_TYPE buff[BUFFER_SIZE] = {0};
  uint64_t size = atoi(argv[2]);

  f = fopen(argv[1], "w");

  fwrite(&size, sizeof(HEADER_TYPE), 1, f);

  HEADER_TYPE i, j, idx;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      idx = i * size + j;
      if (ident) {
        if (j == i) {
          buff[idx % BUFFER_SIZE] = (DATA_TYPE)1;
        } else {
          buff[idx % BUFFER_SIZE] = (DATA_TYPE)0;
        }
      } else {
        buff[idx % BUFFER_SIZE] = rand() % MAX_VAL;
      }
      if ((idx + 1) % BUFFER_SIZE == 0) {
        fwrite(buff, sizeof(DATA_TYPE), BUFFER_SIZE, f);
      }
    }
  }
  idx++;

  if (idx % BUFFER_SIZE != 0) {
    fwrite(buff, sizeof(DATA_TYPE), idx % BUFFER_SIZE, f);
  }

  fclose(f);

  return 0;
}

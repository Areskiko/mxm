#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SEED 2216
#define SIZES 13
#define BUFFER_SIZE 256
#define MAX_VAL 256
#define HEADER_SIZE 4
#define FILENAME_LENGTH 256

int main(int argc, char **argv) {

  if (argc < 2 || argc > 3) {
    fprintf(stderr, "Usage: %s DATA_DIR_PATH [ident]", argv[0]);
    return 1;
  }

  srand(SEED);
  uint64_t sizes[SIZES] = {2,    4,    8,    16,   128,   256,  512,
                           1024, 2048, 4096, 8192, 16384, 32768};

  FILE *f;
  char *file_name = malloc(FILENAME_LENGTH);
  uint8_t buff[BUFFER_SIZE] = {0};

  for (size_t i = 0; i < SIZES; i++) {
    if (argc == 2) {
      snprintf(file_name, FILENAME_LENGTH, "%s/%llu.dat", argv[1], sizes[i]);
    } else {
      snprintf(file_name, FILENAME_LENGTH, "%s/%llu.ident.dat", argv[1],
               sizes[i]);
    }
    f = fopen(file_name, "w");

    fwrite(&sizes[i], sizeof(uint64_t), 1, f);

    size_t j;
    int k = 0;
    for (j = 0; j < sizes[i] * sizes[i]; j++) {
      if (argc == 3) {
        if (j % (sizes[i] + k) == 0) {
          buff[j % BUFFER_SIZE] = 1;
          k++;
        } else {
          buff[j % BUFFER_SIZE] = 0;
        }
      } else {
        buff[j % BUFFER_SIZE] = rand() % MAX_VAL;
      }
      if ((j + 1) % BUFFER_SIZE == 0) {
        fwrite(buff, sizeof(uint8_t), BUFFER_SIZE, f);
      }
    }

    if (j % BUFFER_SIZE != 0) {
      fwrite(buff, sizeof(uint8_t), j % BUFFER_SIZE, f);
    }

    fclose(f);
  }

  return 0;
}

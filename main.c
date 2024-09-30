#include "mxm.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  if (argc != 4) {
    fprintf(stderr, "Usage: %s matrixA matrixB matrixC\n", argv[0]);
    return 1;
  }

  uint8_t *A, *B;

  FILE *fa = fopen(argv[1], "r");
  FILE *fb = fopen(argv[2], "r");
  uint32_t na, nb;
  fread(&na, sizeof(uint32_t), 1, fa);
  fread(&nb, sizeof(uint32_t), 1, fb);

  if (na != nb) {
    fprintf(stderr, "Matrices are of different sizes: %u and %u", na, nb);
    return 1;
  }

  A = malloc(na * na * sizeof(uint8_t));
  fread(A, sizeof(uint8_t), na * na, fa);
  fclose(fa);

  B = malloc(nb * nb * sizeof(uint8_t));
  fread(B, sizeof(uint8_t), nb * nb, fb);
  fclose(fb);

  uint8_t *C = mxm(A, B, na);

  FILE *fc = fopen(argv[3], "w");
  fwrite(&na, sizeof(uint32_t), 1, fc);
  fwrite(C, sizeof(uint8_t), na * na, fc);
  fflush(fc);

  free(A);
  free(B);
  free(C);

  return 0;
}

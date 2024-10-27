#include "mxm.h"
#include "sizes.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

// #define PRINT_MATRIX

#define CMD_LENGTH 512

#ifndef N
#define N 128
#endif

uint64_t NN = (uint64_t) N  * (uint64_t) N;

int main(int argc, char **argv) {

  if (argc != 4) {
    fprintf(stderr, "Usage: %s matrixA matrixB matrixC\n", argv[0]);
    return 1;
  }

  DATA_TYPE *A, *B;

  FILE *fa = fopen(argv[1], "r");
  FILE *fb = fopen(argv[2], "r");
  HEADER_TYPE dump;
  fread(&dump, sizeof(HEADER_TYPE), 1, fa);
  fread(&dump, sizeof(HEADER_TYPE), 1, fb);

  A = malloc(NN * sizeof(DATA_TYPE));
  fread(A, sizeof(DATA_TYPE), NN, fa);
  fclose(fa);

#ifdef PRINT_MATRIX
  printf("Matrix A:\n");
  for (int i = 0; i < na; i++) {
    for (int j = 0; j < na; j++) {
      printf("%llu\t", A[i * na + j]);
    }
    printf("\n");
  }
  printf("\n");
#endif

  B = malloc(NN * sizeof(DATA_TYPE));
  fread(B, sizeof(DATA_TYPE), NN, fb);
  fclose(fb);

#ifdef PRINT_MATRIX
  printf("Matrix B:\n");
  for (int i = 0; i < nb; i++) {
    for (int j = 0; j < nb; j++) {
      printf("%llu\t", B[i * na + j]);
    }
    printf("\n");
  }
  printf("\n");
#endif

  DATA_TYPE *C = mxm(A, B);

#ifdef PRINT_MATRIX
  printf("Matrix C:\n");
  for (int i = 0; i < nb; i++) {
    for (int j = 0; j < nb; j++) {
      printf("%llu\t", C[i * na + j]);
    }
    printf("\n");
  }
  printf("\n");
#endif

  FILE *fc = fopen(argv[3], "w");
  fwrite(&dump, sizeof(HEADER_TYPE), 1, fc);
  fwrite(C, sizeof(DATA_TYPE), NN, fc);
  fflush(fc);

  free(A);
  free(B);
  free(C);

  return 0;
}

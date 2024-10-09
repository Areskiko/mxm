#include "mxm.h"
#include "sizes.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

// #define PRINT_MATRIX

#define CMD_LENGTH 256

int main(int argc, char **argv) {

  if (argc != 4) {
    fprintf(stderr, "Usage: %s matrixA matrixB matrixC\n", argv[0]);
    return 1;
  }

  DATA_TYPE *A, *B;

  FILE *fa = fopen(argv[1], "r");
  FILE *fb = fopen(argv[2], "r");
  HEADER_TYPE na, nb;
  fread(&na, sizeof(HEADER_TYPE), 1, fa);
  fread(&nb, sizeof(HEADER_TYPE), 1, fb);

  if (na != nb) {
    fprintf(stderr, "Matrices are of different sizes: %llu and %llu", na, nb);
    return 1;
  }

  A = malloc(na * na * sizeof(DATA_TYPE));
  fread(A, sizeof(DATA_TYPE), na * na, fa);
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

  B = malloc(nb * nb * sizeof(DATA_TYPE));
  fread(B, sizeof(DATA_TYPE), nb * nb, fb);
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

#ifdef LTO
  DATA_TYPE *C = mxm(A, B);
#endif
#ifndef LTO
  char *cmd = malloc(CMD_LENGTH * sizeof(char));
  sprintf(cmd, "bash -c '$CC --shared -fPIC -O3 -o ./workdir/libmxm.so mxm.c -DN=%llu'", na);
  if (system(cmd)) {
    fprintf(stderr, "Failed to invoke compiler\n");
    free(A);
    free(B);
    return 1;
  }

  errno = 0;
  void *libmxm = dlopen("./workdir/libmxm.so", RTLD_NOW);
  if (errno != 0) {
    printf("dlopen ERRNO: %d\n", errno);
  }
  printf("dlopen complete\n");
  mxm_func dyn_mxm = dlsym(libmxm, "mxm");
  if (errno != 0) {
    printf("dlsym ERRNO: %d\n", errno);
  }
  printf("dlsym complete\n");

  DATA_TYPE *C = (*dyn_mxm)(A, B, "Starting mxm with %d");
  if (errno != 0) {
    printf("mxm ERRNO: %d\n", errno);
  }
  printf("mxm complete\n");

  dlclose(libmxm);
  if (errno != 0) {
    printf("dlclose ERRNO: %d\n", errno);
  }
  printf("dlclose complete\n");
#endif

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
  fwrite(&na, sizeof(HEADER_TYPE), 1, fc);
  fwrite(C, sizeof(DATA_TYPE), na * na, fc);
  fflush(fc);

  free(A);
  free(B);
  free(C);

  return 0;
}

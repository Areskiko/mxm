#include "mxm.h"
#include "sizes.h"
#include <stdio.h>
#include <stdlib.h>

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef INSTRUMENT
#include <time.h>
#endif

#define CMD_LENGTH 512

int main(int argc, char **argv) {

  if (argc != 4) {
    fprintf(stderr, "Usage: %s matrixA matrixB matrixC\n", argv[0]);
    return 1;
  }

#ifdef INSTRUMENT
  clock_t start = clock();
#endif

  DATA_TYPE *A, *B;

  FILE *fa = fopen(argv[1], "r");
  FILE *fb = fopen(argv[2], "r");
  HEADER_TYPE na, nb;
  fread(&na, sizeof(HEADER_TYPE), 1, fa);
  fread(&nb, sizeof(HEADER_TYPE), 1, fb);

#ifdef INSTRUMENT
  clock_t headers = clock();
#endif

  if (na != nb) {
    fprintf(stderr, "Matrices are of different sizes: %llu and %llu", na, nb);
    return 1;
  }

  A = malloc(na * na * sizeof(DATA_TYPE));
  fread(A, sizeof(DATA_TYPE), na * na, fa);
  fclose(fa);

#ifdef INSTRUMENT
  clock_t read_a = clock();
#endif

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

#ifdef INSTRUMENT
  clock_t read_b = clock();
#endif

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

  char *cmd = malloc(CMD_LENGTH * sizeof(char));
  char *workdir = getenv("WORK_DIR");
  char *lib_file = malloc(CMD_LENGTH * sizeof(char));
  sprintf(lib_file, "%s/libmxm.so", workdir);
  sprintf(cmd,
          "bash -c '$CC --shared -fPIC $CC_FLAGS -O3 -o %s mxm.c "
          "-DN=%llu'",
          lib_file, na);
  if (system(cmd)) {
    fprintf(stderr, "Failed to invoke compiler\n");
    free(A);
    free(B);
    return 1;
  }

  void *libmxm = dlopen(lib_file, RTLD_NOW);
  mxm_func dyn_mxm = dlsym(libmxm, "mxm");

#ifdef INSTRUMENT
  clock_t lib_load = clock();
#endif

  DATA_TYPE *C = (*dyn_mxm)(A, B);

#ifdef INSTRUMENT
  clock_t compute = clock();
#endif

  dlclose(libmxm);

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

#ifdef INSTRUMENT
  clock_t write_c = clock();
#endif

  free(A);
  free(B);
  free(C);

#ifdef INSTRUMENT
  // Impl, matrix, header, read_a, read_b, lib_load, compute, write_c

  printf("dynamic_tiling,%llu,%f,%f,%f,%f,%f,%f\n", na,
         (double)(headers - start) / CLOCKS_PER_SEC,
         (double)(read_a - headers) / CLOCKS_PER_SEC,
         (double)(read_b - read_a) / CLOCKS_PER_SEC,
         (double)(lib_load - read_b) / CLOCKS_PER_SEC,
         (double)(compute - lib_load) / CLOCKS_PER_SEC,
         (double)(write_c - compute) / CLOCKS_PER_SEC);
#endif
  return 0;
}

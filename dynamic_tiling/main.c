#include "mxm.h"
#include "sizes.h"
#include <stdio.h>
#include <stdlib.h>

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef INSTRUMENT
#include <time.h>
struct timespec start, headers, read_a, read_b, lib_load, compute, write_c;
#endif

#define CMD_LENGTH 512

int main(int argc, char **argv) {

  if (argc != 4) {
    fprintf(stderr, "Usage: %s matrixA matrixB matrixC\n", argv[0]);
    return 1;
  }

#ifdef INSTRUMENT
  clock_gettime(CLOCK_MONOTONIC, &start);
#endif

  DATA_TYPE *A, *B;

  FILE *fa = fopen(argv[1], "r");
  FILE *fb = fopen(argv[2], "r");
  HEADER_TYPE na, nb;
  fread(&na, sizeof(HEADER_TYPE), 1, fa);
  fread(&nb, sizeof(HEADER_TYPE), 1, fb);

#ifdef INSTRUMENT
  clock_gettime(CLOCK_MONOTONIC, &headers);
#endif

  if (na != nb) {
    fprintf(stderr, "Matrices are of different sizes: %llu and %llu", na, nb);
    return 1;
  }

  A = malloc(na * na * sizeof(DATA_TYPE));
  fread(A, sizeof(DATA_TYPE), na * na, fa);
  fclose(fa);

#ifdef INSTRUMENT
  clock_gettime(CLOCK_MONOTONIC, &read_a);
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
  clock_gettime(CLOCK_MONOTONIC, &read_b);
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
  clock_gettime(CLOCK_MONOTONIC, &lib_load);
#endif

  DATA_TYPE *C = (*dyn_mxm)(A, B);

#ifdef INSTRUMENT
  clock_gettime(CLOCK_MONOTONIC, &compute);
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
  clock_gettime(CLOCK_MONOTONIC, &write_c);
#endif

  free(A);
  free(B);
  free(C);

#ifdef INSTRUMENT
  // Impl, matrix, header, read_a, read_b, lib_load, compute, write_c

#define NANO 1000000000.0
  printf("dynamic_tiling,%llu,%f,%f,%f,%f,%f,%f\n", na,
         (headers.tv_sec - start.tv_sec) +
             ((headers.tv_nsec - start.tv_nsec) / NANO),
         (read_a.tv_sec - headers.tv_sec) +
             ((read_a.tv_nsec - headers.tv_nsec) / NANO),
         (read_b.tv_sec - read_a.tv_sec) +
             ((read_b.tv_nsec - read_a.tv_nsec) / NANO),
         (lib_load.tv_sec - read_b.tv_sec) +
             ((lib_load.tv_nsec - read_b.tv_nsec) / NANO),
         (compute.tv_sec - lib_load.tv_sec) +
             ((compute.tv_nsec - lib_load.tv_nsec) / NANO),
         (write_c.tv_sec - compute.tv_sec) +
             ((write_c.tv_nsec - compute.tv_nsec) / NANO));
#endif
  return 0;
}

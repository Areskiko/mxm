#include "mxm.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  if (argc != 4) {
    fprintf(stderr, "Usage: %s [matrix size] [matrix A seed] [matrix B seed]\n",
            argv[0]);
    return 1;
  }

  int M = atoi(argv[1]);

  int *A, *B;

  A = malloc(M*M * sizeof(int));
  for (int i = 0; i < M * M; i++) {
 	A[i] = i * atoi(argv[2]) % 1024;
  }

  B = malloc(M*M * sizeof(int));
  for (int i = 0; i < M * M; i++) {
 	B[i] = i * atoi(argv[3]) % 1024;
  }

  int *C = mxm(A, B, M);

  fprintf(stdout, "First element in C: %d", C[0]);

  return 0;
}

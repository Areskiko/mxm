#include "mxm.h"
#include "sizes.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef PRINT_MATH
#include <stdio.h>
#endif

DATA_TYPE *mxm(DATA_TYPE *A, DATA_TYPE *B, HEADER_TYPE N) {
  DATA_TYPE *C = calloc(N * N, sizeof(DATA_TYPE));
  for (uint64_t i = 0; i < N; i++) {
    for (uint64_t j = 0; j < N; j++) {

      uint64_t c_index = i * N + j;
      C[c_index] = 0;
#ifdef PRINT_MATH
        printf("\n\nOperating on C[%llu]\n", c_index);
#endif

      for (uint64_t k = 0; k < N; k++) {

        uint64_t a_index = i * N + k;
        uint64_t b_index = k * N + j;
        DATA_TYPE a = A[a_index];
        DATA_TYPE b = B[b_index];
        DATA_TYPE c = C[c_index];
#ifdef PRINT_MATH
        printf("Multiplying A[%llu]: %llu and B[%llu]: %llu (=%llu) and adding to C[%llu]: %llu (=%llu)\n", a_index, a, b_index, b, a*b, c_index, c, a*b+c);
#endif
        C[c_index] = a * b + c;
      }
#ifdef PRINT_MATH
        printf("C[%llu] = %llu\n", c_index, C[c_index]);
#endif
    }
  }
  return C;
}

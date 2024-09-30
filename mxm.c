#include "mxm.h"
#include <stdlib.h>

uint8_t *mxm(uint8_t *A, uint8_t *B, uint64_t N) {
  uint8_t *C = calloc(N * N, sizeof(uint8_t));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {

      int c_index = i * N + j;
      C[c_index] = 0;

      for (int k = 0; k < N; k++) {

        int a_index = i * N + k;
        int b_index = k * N + j;
        uint8_t a = A[a_index];
        uint8_t b = B[b_index];
        uint8_t c = C[c_index];
        C[c_index] = (uint8_t) ((((a * b) % 256) + c) % 256);
      }
    }
  }
  return C;
}

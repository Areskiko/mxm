#include "mxm.h"
#include "sizes.h"
#include <stdlib.h>

#define BLOCK_SIZE 4096

_Alignas(BLOCK_SIZE) DATA_TYPE localA[BLOCK_SIZE][BLOCK_SIZE];
_Alignas(BLOCK_SIZE) DATA_TYPE localB[BLOCK_SIZE][BLOCK_SIZE];
_Alignas(BLOCK_SIZE) DATA_TYPE localC[BLOCK_SIZE][BLOCK_SIZE];

DATA_TYPE *mxm(DATA_TYPE *A, DATA_TYPE *B, HEADER_TYPE N) {
  size_t blockNum = N / BLOCK_SIZE;
  DATA_TYPE *C = calloc(N * N, sizeof(DATA_TYPE));

  // Traverse blocks.
  // #pragma omp parallel for
  for (size_t bi = 0; bi < blockNum; bi++) {
    for (size_t bj = 0; bj < blockNum; bj++) {

      // Clear localC.
      for (size_t i = 0; i < BLOCK_SIZE; i++) {
        for (size_t j = 0; j < BLOCK_SIZE; j++) {
          localC[i][j] = 0;
        }
      }

      for (size_t bk = 0; bk < blockNum; bk++) {
        // Copy local block.
        for (size_t i = 0; i < BLOCK_SIZE; i++) {
          for (size_t j = 0; j < BLOCK_SIZE; j++) {
            size_t aIdx = bi * BLOCK_SIZE * blockNum * BLOCK_SIZE +
                          i * blockNum * BLOCK_SIZE + bk * BLOCK_SIZE + j;
            size_t bIdx = bk * BLOCK_SIZE * blockNum * BLOCK_SIZE +
                          i * blockNum * BLOCK_SIZE + bj * BLOCK_SIZE + j;
            localA[i][j] = A[aIdx];
            localB[i][j] = B[bIdx];
          }
        }

        // Block GEMM.
        for (size_t i = 0; i < BLOCK_SIZE; i++) {
          for (size_t k = 0; k < BLOCK_SIZE; k++) {
            // #pragma omp simd
            for (size_t j = 0; j < BLOCK_SIZE; j++) {
              localC[i][j] += localA[i][k] * localB[k][j];
            }
          }
        }
      }

      // Copy localC back.
      for (size_t i = 0; i < BLOCK_SIZE; i++) {
        for (size_t j = 0; j < BLOCK_SIZE; j++) {
          size_t cIdx = bi * BLOCK_SIZE * blockNum * BLOCK_SIZE +
                        i * blockNum * BLOCK_SIZE + bj * BLOCK_SIZE + j;
          C[cIdx] = localC[i][j];
        }
      }
    }
  }
  return C;
}

#include "mxm.h"
#include "sizes.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef PRINT_MATH
#include <stdio.h>
#endif

#include <stdint.h>
#include <stdlib.h>

#define BLOCK_SIZE 8

_Alignas(64) int localA[BLOCK_SIZE][BLOCK_SIZE];
_Alignas(64) int localB[BLOCK_SIZE][BLOCK_SIZE];
_Alignas(64) int localC[BLOCK_SIZE][BLOCK_SIZE];

DATA_TYPE *mxm(DATA_TYPE *A, DATA_TYPE *B, HEADER_TYPE N) {
  int blockNum = N / BLOCK_SIZE;
  DATA_TYPE *C = calloc(N * N, sizeof(DATA_TYPE));

// Traverse blocks.
#pragma omp parallel for
  for (size_t bi = 0; bi < blockNum; bi++) {
    for (size_t bj = 0; bj < blockNum; bj++) {
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
#pragma omp simd
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

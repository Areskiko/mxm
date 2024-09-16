#include "mxm.h"
#include <stdlib.h>

int* mxm(int* A, int* B, int N)  {
	int *C = malloc(N*N * sizeof(int));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)	{

			int c_index = i * N + j;
			C[c_index] = 0;

			for (int k = 0; k < N; k++) {

				int a_index = i * N + k;
				int b_index = k * N + j;
				C[c_index] += A[a_index] * B[b_index];

			}
		}
	}
	return C;
}

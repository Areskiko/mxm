#ifndef MXM
#define MXM

// Carry out a M x M matrix multiplication
#include <stdint.h>
#include "sizes.h"
DATA_TYPE* mxm(DATA_TYPE*, DATA_TYPE*);
typedef DATA_TYPE* (*mxm_func)(DATA_TYPE*, DATA_TYPE*);

#endif

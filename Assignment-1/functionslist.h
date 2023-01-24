#ifndef FUNCTIONLIST_H
#define FUNCTIONLIST_H

#include "functions.h"
// Function pointer type
typedef char* (*FunctionPtr)(void*);

FunctionPtr functionList[NUM_URLS];

// Array of function pointers
FunctionPtr functionList[NUM_URLS] = {
  [IND_ROOT] = &func_root,
  [IND_SQUARE] = &func_square,
  [IND_CUBE] = &func_cube,
  [IND_HELLOWORLD] = &func_helloworld,
  [IND_PINGPONG] = &func_pingpong,
  [IND_PRIME] = &func_prime,
  [IND_FIBONACCI] = &func_fibonacci,
};

#endif

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define NUM_URLS 7
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <civetweb.h>


// Function prototypes
char* func_root(void*);
char* func_square(void*);
char* func_cube(void*);
char* func_helloworld(void*);
char* func_prime(void*);
char* func_pingpong(void*);
char* func_fibonacci(void*);
int get_index(const char* url);

static const char* urls[NUM_URLS] = {
    "/",
    "/square",
    "/cube",
    "/helloworld",
    "/pingpong",
    "/arithmetic/prime",
    "/arithmetic/fibonacci"
};

enum {
    IND_ROOT = 0,
    IND_SQUARE,
    IND_CUBE,
    IND_HELLOWORLD,
    IND_PINGPONG,
    IND_PRIME,
    IND_FIBONACCI
};

#endif

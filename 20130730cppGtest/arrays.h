#ifndef __ARRAYS_H
#define __ARRAYS_H

#include <limits.h>
#include <stddef.h>

int array_max(int *arr, size_t size);
int array_min(int *arr, size_t size);
int array_sum(int *arr, size_t size);
int array_gcd(int *arr, size_t size);

size_t find_arrmin_index(int *arr, size_t size, int except = INT_MAX);

void array_copy(int *dest, size_t dsize, int *src, size_t ssize);

#endif

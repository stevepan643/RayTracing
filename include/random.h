#ifndef RANDOM_H
#define RANDOM_H

#include <stdlib.h>

double random_double(void);
double random_double_range(double min, double max);

#ifdef RANDOM_IMPLEMENTATION

double random_double(void) { return (double)rand() / (RAND_MAX + 1.0); }
double random_double_range(double min, double max) {
  return min + (max - min) * random_double();
}

#endif
#endif

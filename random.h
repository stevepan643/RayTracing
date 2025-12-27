#ifndef RANDOM_H
#define RANDOM_H

#include <stdlib.h>

inline double random_double() { return (double)rand() / (RAND_MAX + 1.0); }
inline double random_double_range(double min, double max) {
  return min + (max - min) * random_double();
}

#endif

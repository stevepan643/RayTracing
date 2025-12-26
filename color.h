#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <stdio.h>

typedef vec3 color;

void write_color(FILE *out, color c) {
  double r = c.x, g = c.y, b = c.z;
  int rbyte = (int)(255.999 * r), gbyte = (int)(255.999 * g),
      bbyte = (int)(255.999 * b);
  fprintf(out, "%d %d %d\n", rbyte, gbyte, bbyte);
}

#endif

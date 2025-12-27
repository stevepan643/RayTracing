#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"
#include <stdio.h>

typedef vec3 color;

void write_color(FILE *out, color c) {
  double r = c.x, g = c.y, b = c.z;
  static interval in = (interval){0.000, 0.999};
  int rbyte = (int)(255.999 * interval_clamp(in, r)),
      gbyte = (int)(255.999 * interval_clamp(in, g)),
      bbyte = (int)(255.999 * interval_clamp(in, b));
  fprintf(out, "%d %d %d\n", rbyte, gbyte, bbyte);
}

#endif

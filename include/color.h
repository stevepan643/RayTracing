#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"
#include <stdio.h>

typedef vec3 color;
double linear_to_gamma(double linear_component);
void write_color(FILE *out, color c);

#ifdef COLOR_IMPLEMENTATION

double linear_to_gamma(double linear_component) {
  return linear_component > 0 ? sqrt(linear_component) : 0;
}
void write_color(FILE *out, color c) {
  double r = c.x, g = c.y, b = c.z;
  r = linear_to_gamma(r);
  g = linear_to_gamma(g);
  b = linear_to_gamma(b);
  static interval in = {0.0, 0.999};
  int ri = (int)(255.999 * interval_clamp(in, r));
  int gi = (int)(255.999 * interval_clamp(in, g));
  int bi = (int)(255.999 * interval_clamp(in, b));
  fprintf(out, "%d %d %d\n", ri, gi, bi);
}

#endif
#endif

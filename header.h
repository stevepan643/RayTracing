#ifndef HEADER_H
#define HEADER_H

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#include <math.h>
#include <stdlib.h>

const double infinity = INFINITY;
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
  return degrees * pi / 180.0;
}
#endif

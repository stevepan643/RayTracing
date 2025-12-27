#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "material.h"
#include "vec3.h"
#include <math.h>
#include <stdbool.h>

typedef struct sphere {
  hittable super;
  point3 center;
  double radius;
  material *mat;
} sphere;

sphere sphere_new(point3 center, double radius);

#endif

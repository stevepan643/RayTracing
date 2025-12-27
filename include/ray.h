#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct ray {
  point3 orig;
  vec3 dir;
} ray;

ray ray_from(point3 origin, vec3 direction);
point3 ray_at(ray r, double t);

#ifdef RAY_IMPLEMENTATION

ray ray_from(point3 origin, vec3 direction) { return (ray){origin, direction}; }
point3 ray_at(ray r, double t) {
  return vec3_add(r.orig, vec3_mul_doub(r.dir, t));
}

#endif
#endif

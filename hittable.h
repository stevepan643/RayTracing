#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

typedef struct hit_record {
  point3 p;
  vec3 normal;
  double t;
} hit_record;

typedef struct hittable hittable;
struct hittable {
  bool (*hit)(hittable *obj, ray r, double ray_tmin, double ray_tmax,
              hit_record *rec);
};

#endif

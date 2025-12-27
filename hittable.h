#ifndef HITTABLE_H
#define HITTABLE_H

#include "interval.h"
#include "ray.h"

typedef struct hit_record {
  point3 p;
  vec3 normal;
  double t;
  bool front_face;
} hit_record;

void hit_record_set_face_normal(hit_record *self, ray r, vec3 outward_normal) {
  self->front_face = vec3_dot(r.dir, outward_normal) < 0;
  self->normal =
      self->front_face ? outward_normal : vec3_sub(vec3_zero(), outward_normal);
}

typedef struct hittable hittable;
struct hittable {
  bool (*hit)(hittable *obj, ray r, interval ray_t, hit_record *rec);
};

#endif

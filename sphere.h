#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

#include <math.h>
#include <stdbool.h>

typedef struct sphere {
  hittable super;
  point3 center;
  double radius;
} sphere;

static inline bool sphere_hit(hittable *obj, ray r, interval ray_t,
                              hit_record *rec) {
  sphere self = *(sphere *)obj;

  vec3 oc = vec3_sub(self.center, r.orig);
  double a = vec3_len_pow(r.dir);
  double h = vec3_dot(r.dir, oc);
  double c = vec3_len_pow(oc) - self.radius * self.radius;

  double discriminant = h * h - a * c;
  if (discriminant < 0)
    return false;
  double sqrtd = sqrt(discriminant);

  double root = (h - sqrtd) / a;
  if (!interval_surrounds(ray_t, root)) {
    root = (h + sqrtd) / a;
    if (!interval_surrounds(ray_t, root))
      return false;
  }

  rec->t = root;
  rec->p = ray_at(r, rec->t);
  vec3 outward_normal =
      vec3_div_doub(vec3_sub(rec->p, self.center), self.radius);
  hit_record_set_face_normal(rec, r, outward_normal);

  return true;
}

sphere sphere_new(point3 center, double redius) {
  return (sphere){(hittable){sphere_hit}, center, redius};
}

#endif

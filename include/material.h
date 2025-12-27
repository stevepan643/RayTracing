#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "ray.h"
#include "vec3.h"

typedef struct hit_record hit_record;

typedef struct material material;
struct material {
  bool (*scatter)(material *obj, ray r_in, hit_record *rec, color *attenuation,
                  ray *scattered);
};

typedef struct lambertian {
  material super;
  color albedo;
} lambertian;

lambertian lambertian_new(color albedo);

typedef struct metal {
  material super;
  color albedo;
  double fuzz;
} metal;

metal metal_new(color albedo, double fuzz);

#endif

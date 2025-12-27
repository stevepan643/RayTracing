#include "material.h"
#include "hittable.h"
#include "ray.h"
#include "vec3.h"
#include <stdbool.h>
#include <stdlib.h>

static bool lambertian_scatter(material *obj, ray r_in, hit_record *rec,
                               color *attenuation, ray *scattered) {
  lambertian *self = (lambertian *)obj;

  vec3 scatter_dir =
      vec3_add(rec->normal, vec3_random_on_hemisphere(rec->normal));
  if (vec3_near_zero(scatter_dir))
    scatter_dir = rec->normal;

  *scattered = (ray){rec->p, vec3_unit(scatter_dir)};
  *attenuation = self->albedo;
  return true;
}

lambertian lambertian_new(color albedo) {
  return (lambertian){.super = {lambertian_scatter}, .albedo = albedo};
}

static bool metal_scatter(material *obj, ray r_in, hit_record *rec,
                          color *attenuation, ray *scattered) {
  metal *self = (metal *)obj;

  vec3 reflected = vec3_reflect(vec3_unit(r_in.dir), rec->normal);
  if (self->fuzz > 0) {
    vec3 fuzz_vec =
        vec3_mul_doub(vec3_random_on_hemisphere(rec->normal), self->fuzz);
    reflected = vec3_add(reflected, fuzz_vec);
  }

  *scattered = (ray){rec->p, vec3_unit(reflected)};
  *attenuation = self->albedo;
  return vec3_dot(scattered->dir, rec->normal) > 0;
}

metal metal_new(color albedo, double fuzz) {
  if (fuzz > 1)
    fuzz = 1;
  if (fuzz < 0)
    fuzz = 0;
  return (metal){.super = {metal_scatter}, .albedo = albedo, .fuzz = fuzz};
}

static double reflectance(double cosine, double refraction_index) {
  double r0 = (1 - refraction_index) / (1 + refraction_index);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cosine), 5);
}

static bool dielectric_scatter(material *obj, ray r_in, hit_record *rec,
                               color *attenuation, ray *scattered) {
  dielectric *self = (dielectric *)obj;

  *attenuation = (color){1, 1, 1};
  double ri =
      rec->front_face ? (1.0 / self->refraction_index) : self->refraction_index;

  vec3 unit_direction = vec3_unit(r_in.dir);
  double cos_theta = fmin(vec3_dot(vec3_neg(unit_direction), rec->normal), 1.0);
  double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

  bool cannot_refract = ri * sin_theta > 1.0;
  vec3 dir;

  if (cannot_refract || reflectance(cos_theta, ri) > random_double())
    dir = vec3_reflect(unit_direction, rec->normal);
  else
    dir = vec3_refract(unit_direction, rec->normal, ri);

  *scattered = ray_from(rec->p, dir);
  return true;
}

dielectric dielectric_new(double refraction_index) {
  return (dielectric){(material){dielectric_scatter}, refraction_index};
}

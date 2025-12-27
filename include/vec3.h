#ifndef VEC3_H
#define VEC3_H

#include "random.h"
#include <math.h>
#include <stdbool.h>

typedef struct vec3 {
  double x, y, z;
} vec3;
typedef vec3 point3;

// 声明
vec3 vec3_zero(void);
vec3 vec3_from(double e0, double e1, double e2);
vec3 vec3_neg(vec3 v);
vec3 vec3_add(vec3 v1, vec3 v2);
vec3 vec3_sub(vec3 v1, vec3 v2);
vec3 vec3_mul_vec3(vec3 v1, vec3 v2);
vec3 vec3_div_vec3(vec3 v1, vec3 v2);
vec3 vec3_mul_doub(vec3 v, double d);
vec3 vec3_div_doub(vec3 v, double d);
vec3 vec3_cross(vec3 v1, vec3 v2);
double vec3_dot(vec3 v1, vec3 v2);
double vec3_len_pow(vec3 v);
double vec3_len(vec3 v);
vec3 vec3_unit(vec3 v);
vec3 vec3_random(void);
vec3 vec3_random_from(double min, double max);
vec3 vec3_random_unit_vec3(void);
vec3 vec3_random_on_hemisphere(vec3 normal);
bool vec3_near_zero(vec3 v);
vec3 vec3_reflect(vec3 v, vec3 n);
vec3 vec3_refract(vec3 uv, vec3 n, double etai_over_etat);
vec3 vec3_random_in_unit_disk();

#ifdef VEC3_IMPLEMENTATION

vec3 vec3_zero(void) { return (vec3){0, 0, 0}; }
vec3 vec3_from(double e0, double e1, double e2) { return (vec3){e0, e1, e2}; }
vec3 vec3_neg(vec3 v) { return (vec3){-v.x, -v.y, -v.z}; }
vec3 vec3_add(vec3 v1, vec3 v2) {
  return (vec3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}
vec3 vec3_sub(vec3 v1, vec3 v2) {
  return (vec3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}
vec3 vec3_mul_vec3(vec3 v1, vec3 v2) {
  return (vec3){v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}
vec3 vec3_div_vec3(vec3 v1, vec3 v2) {
  return (vec3){v1.x / v2.x, v1.y / v2.y, v1.z / v2.z};
}
vec3 vec3_mul_doub(vec3 v, double d) {
  return (vec3){v.x * d, v.y * d, v.z * d};
}
vec3 vec3_div_doub(vec3 v, double d) { return vec3_mul_doub(v, 1.0 / d); }
vec3 vec3_cross(vec3 v1, vec3 v2) {
  return (vec3){v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x};
}
double vec3_dot(vec3 v1, vec3 v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
double vec3_len_pow(vec3 v) { return vec3_dot(v, v); }
double vec3_len(vec3 v) { return sqrt(vec3_len_pow(v)); }
vec3 vec3_unit(vec3 v) { return vec3_div_doub(v, vec3_len(v)); }

vec3 vec3_random(void) {
  return (vec3){random_double(), random_double(), random_double()};
}
vec3 vec3_random_from(double min, double max) {
  return (vec3){random_double_range(min, max), random_double_range(min, max),
                random_double_range(min, max)};
}

vec3 vec3_random_unit_vec3(void) {
  while (1) {
    vec3 p = vec3_random();
    double len2 = vec3_len_pow(p);
    if (len2 > 1e-160 && len2 <= 1)
      return vec3_div_doub(p, sqrt(len2));
  }
}
vec3 vec3_random_on_hemisphere(vec3 normal) {
  vec3 p = vec3_random_unit_vec3();
  if (vec3_dot(p, normal) > 0)
    return p;
  return vec3_mul_doub(p, -1.0);
}
bool vec3_near_zero(vec3 v) {
  double s = 1e-8;
  return fabs(v.x) < s && fabs(v.y) < s && fabs(v.z) < s;
}
vec3 vec3_reflect(vec3 v, vec3 n) {
  return vec3_sub(v, vec3_mul_doub(n, 2.0 * vec3_dot(v, n)));
}

vec3 vec3_refract(vec3 uv, vec3 n, double etai_over_etat) {
  double cos_theta = fmin(vec3_dot(vec3_neg(uv), n), 1.0);
  vec3 r_out_prep =
      vec3_mul_doub(vec3_add(uv, vec3_mul_doub(n, cos_theta)), etai_over_etat);
  vec3 r_out_parallel =
      vec3_mul_doub(n, -sqrt(fabs(1.0 - vec3_len_pow(r_out_prep))));
  return vec3_add(r_out_prep, r_out_parallel);
}
vec3 vec3_random_in_unit_disk() {
  while (true) {
    vec3 p =
        vec3_from(random_double_range(-1, 1), random_double_range(-1, 1), 0);
    if (vec3_len_pow(p) < 1)
      return p;
  }
}

#endif // VEC3_IMPLEMENTATION
#endif // VEC3_H

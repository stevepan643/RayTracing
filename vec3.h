#ifndef VEC3_H
#define VEC3_H

#include <math.h>

typedef struct vec3 {
  double x, y, z;
} vec3;
typedef vec3 point3;

vec3 vec3_zero() { return (vec3){0, 0, 0}; }
vec3 vec3_from(double e0, double e1, double e2) { return (vec3){e0, e1, e2}; }

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
vec3 vec3_mul_doub(vec3 v1, double d) {
  return (vec3){v1.x * d, v1.y * d, v1.z * d};
}
vec3 vec3_div_doub(vec3 v1, double d) { return vec3_mul_doub(v1, 1 / d); }
vec3 vec3_cross(vec3 v1, vec3 v2) {
  return (vec3){v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x};
}
double vec3_dot(vec3 v1, vec3 v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
double vec3_len_pow(vec3 v) { return v.x * v.x + v.y * v.y + v.z * v.z; }
double vec3_len(vec3 v) { return sqrt(vec3_len_pow(v)); }
vec3 vec3_unit(vec3 v) { return vec3_div_doub(v, vec3_len(v)); }

#endif

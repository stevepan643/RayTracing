#ifndef HITTABLE_H
#define HITTABLE_H

#include "interval.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

// 前置声明
// typedef struct material material;

typedef struct hit_record {
  point3 p;
  vec3 normal;
  material *mat;
  double t;
  bool front_face;
} hit_record;

void hit_record_set_face_normal(hit_record *self, ray r, vec3 outward_normal);

typedef struct hittable hittable;
struct hittable {
  bool (*hit)(hittable *obj, ray r, interval ray_t, hit_record *rec);
};

typedef struct hittable_list {
  hittable super;
  hittable **objs;
  int size;
  int capacity;
} hittable_list;

// sphere 对象
typedef struct sphere {
  hittable super;
  point3 center;
  double radius;
  material *mat;
} sphere;

// 声明
sphere sphere_new(point3 center, double radius, material *mat);
hittable_list hittable_list_new(void);
void hittable_list_clear(hittable_list *self);
void hittable_list_reserve(hittable_list *self, int newcap);
void hittable_list_add(hittable_list *self, hittable *obj);

#ifdef HITTABLE_IMPLEMENTATION

// ---------- hit_record ----------
void hit_record_set_face_normal(hit_record *self, ray r, vec3 outward_normal) {
  self->front_face = vec3_dot(r.dir, outward_normal) < 0;
  self->normal =
      self->front_face ? outward_normal : vec3_mul_doub(outward_normal, -1.0);
}

// ---------- sphere ----------
static bool sphere_hit(hittable *obj, ray r, interval ray_t, hit_record *rec) {
  sphere self = *(sphere *)obj;
  vec3 oc = vec3_sub(r.orig, self.center);
  double a = vec3_len_pow(r.dir);
  double half_b = vec3_dot(r.dir, oc);
  double c = vec3_len_pow(oc) - self.radius * self.radius;
  double discriminant = half_b * half_b - a * c;
  if (discriminant < 0)
    return false;
  double sqrtd = sqrt(discriminant);

  double root = (-half_b - sqrtd) / a;
  if (!interval_surrounds(ray_t, root)) {
    root = (-half_b + sqrtd) / a;
    if (!interval_surrounds(ray_t, root))
      return false;
  }

  rec->t = root;
  rec->p = ray_at(r, rec->t);
  vec3 outward_normal =
      vec3_div_doub(vec3_sub(rec->p, self.center), self.radius);
  hit_record_set_face_normal(rec, r, outward_normal);
  rec->mat = self.mat;
  return true;
}

sphere sphere_new(point3 center, double radius, material *mat) {
  return (sphere){(hittable){sphere_hit}, center, radius, mat};
}

// ---------- hittable_list ----------
static bool hittable_list_hit(hittable *obj, ray r, interval ray_t,
                              hit_record *rec) {
  hittable_list *self = (hittable_list *)obj;
  hit_record temp_rec;
  bool hit_anything = false;
  double closest_so_far = ray_t.max;

  for (int i = 0; i < self->size; i++) {
    hittable *o = self->objs[i];
    if (o->hit(o, r, (interval){ray_t.min, closest_so_far}, &temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      *rec = temp_rec;
    }
  }
  return hit_anything;
}

hittable_list hittable_list_new(void) {
  int capacity = 4;
  hittable_list l;
  l.super.hit = hittable_list_hit;
  l.size = 0;
  l.capacity = capacity;
  l.objs = (hittable **)malloc(sizeof(hittable *) * capacity);
  return l;
}

void hittable_list_clear(hittable_list *self) { self->size = 0; }
void hittable_list_reserve(hittable_list *self, int newcap) {
  if (newcap <= self->capacity)
    return;
  self->capacity = newcap;
  self->objs =
      (hittable **)realloc(self->objs, sizeof(hittable *) * self->capacity);
}
void hittable_list_add(hittable_list *self, hittable *obj) {
  if (self->size == self->capacity)
    hittable_list_reserve(self, self->capacity * 2);
  self->objs[self->size++] = obj;
}

#endif // HITTABLE_IMPLEMENTATION
#endif // HITTABLE_H

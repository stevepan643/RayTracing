#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "interval.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct hittable_list {
  hittable super;
  hittable **objs;
  int size;
  int capacity;
} hittable_list;

static inline bool hittable_list_hit(hittable *obj, ray r, interval ray_t,
                                     hit_record *rec) {
  hittable_list *self = (hittable_list *)obj;

  hit_record temp_rec;
  bool hit_anything = false;
  double closest_so_far = ray_t.max;

  for (int i = 0; i < self->size; ++i) {
    hittable *obj = self->objs[i];

    if (obj->hit(obj, r, (interval){ray_t.min, closest_so_far}, &temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      *rec = temp_rec;
    }
  }

  return hit_anything;
}

hittable_list hittable_list_new() {
  int capacity = 4;
  return (hittable_list){(hittable){hittable_list_hit},
                         (hittable **)malloc(sizeof(hittable *) * capacity), 0,
                         capacity};
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

#endif

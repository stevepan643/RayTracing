#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"
#include "random.h"

#include <stdio.h>

typedef struct camera {
  double aspect_ratio;
  int image_width;
  int samples_per_pixel;

  int image_height;
  double pixel_samples_scale;
  point3 center;
  point3 pixel100_loc;
  vec3 pixel_delta_u;
  vec3 pixel_delta_v;
} camera;

void initialize(camera *c);
color ray_color(camera *c, ray r, hittable *world);
ray get_ray(camera *c, int i, int j);
vec3 sample_square(camera *c);

void render(camera *c, hittable *world) {
  initialize(c);
  printf("P3\n%d %d\n255\n", c->image_width, c->image_height);

  for (int j = 0; j < c->image_height; j++) {
    fprintf(stderr, "\rScanlines remaining: %d ", (c->image_height - j));
    for (int i = 0; i < c->image_width; i++) {
      color pixel_color = vec3_zero();
      for (int sample = 0; sample < c->samples_per_pixel; sample++) {
        ray r = get_ray(c, i, j);
        pixel_color = vec3_add(pixel_color, ray_color(c, r, world));
      }
      write_color(stdout, vec3_mul_doub(pixel_color, c->pixel_samples_scale));
    }
  }
  fprintf(stderr, "\rDone.                 \n");
}

void initialize(camera *c) {
  c->image_height = (int)c->image_width / c->aspect_ratio;
  c->image_height = (c->image_height < 1) ? 1 : c->image_height;

  c->pixel_samples_scale = 1.0 / c->samples_per_pixel;

  c->center = vec3_zero();

  double focal_length = 1.0;
  double viewport_height = 2.0;
  double viewport_width =
      viewport_height * ((double)c->image_width / c->image_height);

  vec3 viewport_u = vec3_from(viewport_width, 0, 0);
  vec3 viewport_v = vec3_from(0, -viewport_height, 0);

  c->pixel_delta_u = vec3_div_doub(viewport_u, c->image_width);
  c->pixel_delta_v = vec3_div_doub(viewport_v, c->image_height);

  vec3 viewport_upper_left =
      vec3_sub(vec3_sub(vec3_sub(c->center, vec3_from(0, 0, focal_length)),
                        vec3_div_doub(viewport_u, 2)),
               vec3_div_doub(viewport_v, 2));
  c->pixel100_loc = vec3_add(
      viewport_upper_left,
      vec3_mul_doub(vec3_add(c->pixel_delta_u, c->pixel_delta_v), 0.5));
}

ray get_ray(camera *c, int i, int j) {
  vec3 offset = sample_square(c);
  vec3 pixel_sample =
      vec3_add(c->pixel100_loc,
               vec3_add(vec3_mul_doub(c->pixel_delta_u, (i + offset.x)),
                        vec3_mul_doub(c->pixel_delta_v, (j + offset.y))));
  vec3 ray_orig = c->center;
  vec3 ray_dir = vec3_sub(pixel_sample, ray_orig);

  return (ray){ray_orig, ray_dir};
}

vec3 sample_square(camera *c) {
  return (vec3){random_double() - 0.5, random_double() - 0.5, 0};
}

color ray_color(camera *c, ray r, hittable *world) {
  hit_record rec;
  if (world->hit(world, r, interval_from(0, INFINITY), &rec))
    return vec3_mul_doub(vec3_add(rec.normal, vec3_from(1, 1, 1)), 0.5);
  vec3 unit_direction = vec3_unit(r.dir);
  double a = 0.5 * (unit_direction.y + 1.0);
  return vec3_add(vec3_mul_doub(vec3_from(1.0, 1.0, 1.0), (1.0 - a)),
                  vec3_mul_doub(vec3_from(0.5, 0.7, 1.0), a));
}

#endif

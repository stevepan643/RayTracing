#include "camera.h"
#include "color.h"
#include "interval.h"
#include "random.h"
#include <math.h>
#include <stdio.h>

void initialize(camera *c) {
  c->image_height = (int)(c->image_width / c->aspect_ratio);
  if (c->image_height < 1)
    c->image_height = 1;
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

vec3 sample_square(camera *c) {
  return vec3_from(random_double() - 0.5, random_double() - 0.5, 0);
}

ray get_ray(camera *c, int i, int j) {
  vec3 offset = sample_square(c);
  vec3 pixel_sample = vec3_add(
      c->pixel100_loc, vec3_add(vec3_mul_doub(c->pixel_delta_u, i + offset.x),
                                vec3_mul_doub(c->pixel_delta_v, j + offset.y)));
  return (ray){c->center, vec3_sub(pixel_sample, c->center)};
}

color ray_color(camera *c, ray r, int depth, hittable *world) {
  if (depth <= 0)
    return vec3_zero();
  hit_record rec;
  if (world->hit(world, r, (interval){0.001, INFINITY}, &rec)) {
    ray scattered;
    color attenuation;
    if (rec.mat->scatter(rec.mat, r, &rec, &attenuation, &scattered))
      return vec3_mul_vec3(attenuation,
                           ray_color(c, scattered, depth - 1, world));
    return vec3_zero();
  }
  vec3 unit_direction = vec3_unit(r.dir);
  double t = 0.5 * (unit_direction.y + 1.0);
  return vec3_add(vec3_mul_doub(vec3_from(1.0, 1.0, 1.0), 1.0 - t),
                  vec3_mul_doub(vec3_from(0.5, 0.7, 1.0), t));
}

void render(camera *c, hittable *world) {
  initialize(c);
  printf("P3\n%d %d\n255\n", c->image_width, c->image_height);
  for (int j = 0; j < c->image_height; j++) {
    for (int i = 0; i < c->image_width; i++) {
      color pixel_color = vec3_zero();
      for (int s = 0; s < c->samples_per_pixel; s++) {
        ray r = get_ray(c, i, j);
        pixel_color =
            vec3_add(pixel_color, ray_color(c, r, c->max_depth, world));
      }
      write_color(stdout, vec3_mul_doub(pixel_color, c->pixel_samples_scale));
    }
    fprintf(stderr, "\rScanlines remaining: %d ", c->image_height - j);
  }
  fprintf(stderr, "\rDone. \n");
}

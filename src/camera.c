#include "camera.h"
#include "color.h"
#include "interval.h"
#include "math_help.h"
#include "random.h"
#include <math.h>
#include <stdio.h>

void initialize(camera *c) {
  c->image_height = (int)(c->image_width / c->aspect_ratio);
  c->image_height = (c->image_height < 1) ? 1 : c->image_height;

  c->pixel_samples_scale = 1.0 / c->samples_per_pixel;

  c->center = c->lookfrom;

  double theta = degrees_to_radians(c->vfov);
  double h = tan(theta / 2.0);
  double viewport_height = 2.0 * h * c->focus_dist;
  double viewport_width =
      viewport_height * ((double)c->image_width / c->image_height);

  c->w = vec3_unit(vec3_sub(c->lookfrom, c->lookat));
  c->u = vec3_unit(vec3_cross(c->vup, c->w));
  c->v = vec3_cross(c->w, c->u);

  vec3 viewport_u = vec3_mul_doub(c->u, viewport_width);
  vec3 viewport_v = vec3_mul_doub(vec3_neg(c->v), viewport_height);

  c->pixel_delta_u = vec3_div_doub(viewport_u, c->image_width);
  c->pixel_delta_v = vec3_div_doub(viewport_v, c->image_height);

  vec3 half_u = vec3_div_doub(viewport_u, 2.0);
  vec3 half_v = vec3_div_doub(viewport_v, 2.0);
  vec3 temp = vec3_sub(c->center, vec3_mul_doub(c->w, c->focus_dist));
  vec3 viewport_upper_left = vec3_sub(vec3_sub(temp, half_v), half_u);

  vec3 delta = vec3_mul_doub(vec3_add(c->pixel_delta_u, c->pixel_delta_v), 0.5);
  c->pixel00_loc = vec3_add(viewport_upper_left, delta);

  double defocus_radius =
      c->focus_dist * tan(degrees_to_radians(c->defocus_angle / 2));
  c->defocus_disk_u = vec3_mul_doub(c->u, defocus_radius);
  c->defocus_disk_v = vec3_mul_doub(c->v, defocus_radius);
}

vec3 sample_square(camera *c) {
  return vec3_from(random_double() - 0.5, random_double() - 0.5, 0);
}

point3 defocus_disk_sample(camera *c) {
  vec3 p = vec3_random_in_unit_disk();
  return vec3_add(vec3_add(c->center, vec3_mul_doub(c->defocus_disk_u, p.x)),
                  vec3_mul_doub(c->defocus_disk_v, p.y));
}

ray get_ray(camera *c, int i, int j) {
  vec3 offset = sample_square(c);
  vec3 pixel_sample = vec3_add(
      c->pixel00_loc, vec3_add(vec3_mul_doub(c->pixel_delta_u, i + offset.x),
                               vec3_mul_doub(c->pixel_delta_v, j + offset.y)));
  vec3 ray_orig = (c->defocus_angle <= 0) ? c->center : defocus_disk_sample(c);
  vec3 ray_dir = vec3_sub(pixel_sample, ray_orig);
  return (ray){ray_orig, ray_dir};
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
  color *framebuffer = malloc(c->image_width * c->image_height * sizeof(color));
  int scanlines_done = 0;
#pragma omp parallel for
  for (int j = 0; j < c->image_height; j++) {
    for (int i = 0; i < c->image_width; i++) {
      color pixel_color = vec3_zero();
      for (int s = 0; s < c->samples_per_pixel; s++) {
        ray r = get_ray(c, i, j);
        pixel_color =
            vec3_add(pixel_color, ray_color(c, r, c->max_depth, world));
      }
      framebuffer[j * c->image_width + i] =
          vec3_mul_doub(pixel_color, c->pixel_samples_scale);
    }

#pragma omp atomic
    scanlines_done++;

#pragma omp critical
    fprintf(stderr, "\rScanlines remaining: %d ",
            c->image_height - scanlines_done);
  }

  fprintf(stderr, "\rDone.            \n");
  printf("P3\n%d %d\n255\n", c->image_width, c->image_height);
  for (int j = 0; j < c->image_height; j++) {
    for (int i = 0; i < c->image_width; i++) {
      write_color(stdout, framebuffer[j * c->image_width + i]);
    }
  }
  free(framebuffer);
}

#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "vec3.h"

#include <stdbool.h>
#include <stdio.h>

double hit_sphere(point3 center, double radius, ray r) {
  vec3 oc = vec3_sub(center, r.orig);
  double a = vec3_len_pow(r.dir);
  double h = vec3_dot(r.dir, oc);
  double c = vec3_len_pow(oc) - radius * radius;
  double discriminant = h * h - a * c;

  if (discriminant < 0)
    return -1.0;
  else
    return (h - sqrt(discriminant)) / a;
}

color ray_color(ray r) {
  double t = hit_sphere(vec3_from(0, 0, -1), 0.5, r);
  if (t > 0.0) {
    vec3 N = vec3_unit(vec3_sub(ray_at(r, t), vec3_from(0, 0, -1)));
    return vec3_mul_doub(vec3_from(N.x + 1, N.y + 1, N.z + 1), 0.5);
  }
  vec3 unit_direction = vec3_unit(r.dir);
  double a = 0.5 * (unit_direction.y + 1.0);
  return vec3_add(vec3_mul_doub(vec3_from(1.0, 1.0, 1.0), (1.0 - a)),
                  vec3_mul_doub(vec3_from(0.5, 0.7, 1.0), a));
}

int main() {
  double aspect_ratio = 16.0 / 9.0;
  int image_width = 400;
  int image_height = (int)(image_width / aspect_ratio);
  image_height = (image_height < 1) ? 1 : image_height;

  double focal_length = 1.0;
  double viewport_height = 2.0;
  double viewport_width =
      viewport_height * ((double)image_width / image_height);
  point3 camera_center = vec3_from(0, 0, 0);

  vec3 viewport_u = vec3_from(viewport_width, 0, 0);
  vec3 viewport_v = vec3_from(0, -viewport_height, 0);

  vec3 pixel_delta_u = vec3_div_doub(viewport_u, image_width);
  vec3 pixel_delta_v = vec3_div_doub(viewport_v, image_height);

  vec3 viewport_upper_left =
      vec3_sub(vec3_sub(vec3_sub(camera_center, vec3_from(0, 0, focal_length)),
                        vec3_div_doub(viewport_u, 2)),
               vec3_div_doub(viewport_v, 2));
  vec3 pixel100_loc =
      vec3_add(viewport_upper_left,
               vec3_mul_doub(vec3_add(pixel_delta_u, pixel_delta_v), 0.5));

  printf("P3\n%d %d\n255\n", image_width, image_height);

  for (int j = 0; j < image_height; j++) {
    fprintf(stderr, "\rScanlines remaining: %d ", (image_height - j));
    for (int i = 0; i < image_width; i++) {
      vec3 pixel_center =
          vec3_add(pixel100_loc, vec3_add(vec3_mul_doub(pixel_delta_u, i),
                                          vec3_mul_doub(pixel_delta_v, j)));
      vec3 ray_direction = vec3_sub(pixel_center, camera_center);
      ray r = ray_from(camera_center, ray_direction);
      color pixel_color = ray_color(r);
      write_color(stdout, pixel_color);
    }
  }
  fprintf(stderr, "\rDone.                 \n");
  return 0;
}

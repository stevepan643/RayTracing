#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "vec3.h"

typedef struct camera {
  double aspect_ratio;
  int image_width;
  int image_height;
  int samples_per_pixel;
  int max_depth;
  double pixel_samples_scale;

  double vfov;
  point3 lookfrom;
  point3 lookat;
  vec3 vup;

  double defocus_angle;
  double focus_dist;

  point3 center;
  point3 pixel00_loc;
  vec3 pixel_delta_u;
  vec3 pixel_delta_v;
  vec3 u, v, w;
  vec3 defocus_disk_u;
  vec3 defocus_disk_v;
} camera;

void initialize(camera *c);
ray get_ray(camera *c, int i, int j);
vec3 sample_square(camera *c);
color ray_color(camera *c, ray r, int depth, hittable *world);
void render(camera *c, hittable *world);

#endif

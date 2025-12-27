#define INTERVAL_IMPLEMENTATION
#include "interval.h"

#define RANDOM_IMPLEMENTATION
#include "random.h"

#define VEC3_IMPLEMENTATION
#include "vec3.h"

#define COLOR_IMPLEMENTATION
#include "color.h"

#define RAY_IMPLEMENTATION
#include "ray.h"

#define HITTABLE_IMPLEMENTATION
#include "hittable.h"

#include "camera.h"
#include "material.h"

#include <stdbool.h>
#include <stdio.h>
#include <time.h>

int main() {
  srand((unsigned)time(NULL));

  lambertian material_ground = lambertian_new((color){0.8, 0.8, 0.0});
  lambertian material_center = lambertian_new((color){0.1, 0.2, 0.5});
  metal material_left = metal_new((color){0.8, 0.8, 0.8}, 0);
  metal material_right = metal_new((color){0.8, 0.6, 0.2}, 0.8);

  sphere ground =
      sphere_new(vec3_from(0, -100.5, -1), 100, (material *)&material_ground);
  sphere center =
      sphere_new(vec3_from(0, 0, -1.2), 0.5, (material *)&material_left);
  sphere left =
      sphere_new(vec3_from(-1.0, 0, -1), 0.5, (material *)&material_left);
  sphere right =
      sphere_new(vec3_from(1.0, 0, -1), 0.5, (material *)&material_right);

  hittable_list world = hittable_list_new();

  hittable_list_add(&world, (hittable *)&ground);
  hittable_list_add(&world, (hittable *)&center);
  hittable_list_add(&world, (hittable *)&left);
  hittable_list_add(&world, (hittable *)&right);

  camera cam;
  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 2048;
  cam.max_depth = 1024;

  render(&cam, (hittable *)&world);
  return 0;
}

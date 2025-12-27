#include "camera.h"
#include "header.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#include <stdbool.h>
#include <stdio.h>
#include <time.h>

int main() {
  srand((unsigned)time(NULL));
  sphere s1 = sphere_new(vec3_from(0, 0, -1), 0.5);
  sphere s2 = sphere_new(vec3_from(0, -100.5, -1), 100);
  hittable_list world = hittable_list_new();
  hittable_list_add(&world, (hittable *)&s1);
  hittable_list_add(&world, (hittable *)&s2);

  camera cam;
  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 500;

  render(&cam, (hittable *)&world);
  return 0;
}

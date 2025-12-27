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

void create_random_spheres(hittable_list *world, int min_a, int max_a,
                           int min_b, int max_b) {
  for (int a = min_a; a < max_a; a++) {
    for (int b = min_b; b < max_b; b++) {
      double choose_mat = random_double();
      point3 center =
          vec3_from(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if (vec3_len(vec3_sub(center, vec3_from(4, 0.2, 0))) > 0.9) {
        material *mat = NULL;
        sphere *s = malloc(sizeof(sphere));
        if (!s) {
          fprintf(stderr, "malloc failed\n");
          exit(1);
        }

        if (choose_mat < 0.8) {
          // diffuse
          lambertian *l = malloc(sizeof(lambertian));
          *l = lambertian_new(vec3_mul_vec3(vec3_random(), vec3_random()));
          mat = (material *)l;

        } else if (choose_mat < 0.95) {
          // metal
          metal *m = malloc(sizeof(metal));
          *m = metal_new(vec3_random_from(0.5, 1.0),
                         random_double_range(0, 0.5));
          mat = (material *)m;

        } else {
          // glass
          dielectric *d = malloc(sizeof(dielectric));
          *d = dielectric_new(1.5);
          mat = (material *)d;
        }

        *s = sphere_new(center, 0.2, mat);
        hittable_list_add(world, (hittable *)s);
      }
    }
  }
}

void add_large_spheres(hittable_list *world) {
  dielectric *d = malloc(sizeof(dielectric));
  *d = dielectric_new(1.5);
  sphere *s1 = malloc(sizeof(sphere));
  *s1 = sphere_new(vec3_from(0, 1, 0), 1.0, (material *)d);
  hittable_list_add(world, (hittable *)s1);

  lambertian *l = malloc(sizeof(lambertian));
  *l = lambertian_new(vec3_from(0.4, 0.2, 0.1));
  sphere *s2 = malloc(sizeof(sphere));
  *s2 = sphere_new(vec3_from(-4, 1, 0), 1.0, (material *)l);
  hittable_list_add(world, (hittable *)s2);

  metal *m = malloc(sizeof(metal));
  *m = metal_new(vec3_from(0.7, 0.6, 0.5), 0.0);
  sphere *s3 = malloc(sizeof(sphere));
  *s3 = sphere_new(vec3_from(4, 1, 0), 1.0, (material *)m);
  hittable_list_add(world, (hittable *)s3);
}

void add_ground(hittable_list *world) {
  lambertian *ground_mat = malloc(sizeof(lambertian));
  *ground_mat = lambertian_new(vec3_from(0.5, 0.5, 0.5));

  sphere *ground = malloc(sizeof(sphere));
  *ground = sphere_new(vec3_from(0, -1000, 0), 1000, (material *)ground_mat);

  hittable_list_add(world, (hittable *)ground);
}

void free_world(hittable_list *world) {
  for (int i = 0; i < world->size; i++) {
    sphere *s = (sphere *)world->objs[i];
    free(s->mat);
    free(s);
  }
  free(world->objs);
}

int main() {
  srand((unsigned)time(NULL));

  hittable_list world = hittable_list_new();

  add_ground(&world);
  create_random_spheres(&world, -11, 11, -11, 11);
  add_large_spheres(&world);

  camera cam;
  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 1920;
  cam.samples_per_pixel = 500;
  cam.max_depth = 50;

  cam.vfov = 20;
  cam.lookfrom = vec3_from(13, 2, 3);
  cam.lookat = vec3_from(0, 0, 0);
  cam.vup = vec3_from(0, 1, 0);

  cam.defocus_angle = 0.6;
  cam.focus_dist = 10.0;

  render(&cam, (hittable *)&world);

  free_world(&world);

  return 0;
}

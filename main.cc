
#define _USE_MATH_DEFINES
#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "random.h"
#include"svpng.h"
#include<cmath>
#include<cstdlib>
#include<chrono>
#include<time.h>
#include"./Project2/moving_sphere.h"
using namespace std;
using namespace chrono;
#define    MAXFLOAT    0x1.fffffep+127f
color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0, 0, 0);

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
}
//reconsruct the whole material
hittable_list random_scene() {
    hittable_list world;

    world.add(make_shared<sphere>(
        point3(0, -1000, 0), 1000, make_shared<lambertian>(color(0.5, 0.5, 0.5))));

    int i = 1;
    for (int a = -10; a < 10; a++) {
        for (int b = -10; b < 10; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            if ((center - vec3(4, .2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    world.add(make_shared<moving_sphere>(
                        center, center + vec3(0, random_double(0, .5), 0), 0.0, 1.0, 0.2,
                        make_shared<lambertian>(albedo)));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, .5);
                    world.add(
                        make_shared<sphere>(center, 0.2, make_shared<metal>(albedo, fuzz)));
                }
                else {
                    // glass
                    world.add(make_shared<sphere>(center, 0.2, make_shared<dielectric>(1.5)));
                }
            }
        }
    }

    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(
        point3(-4, 1, 0), 1.0, make_shared<lambertian>(color(0.4, 0.2, 0.1))));
    world.add(make_shared<sphere>(
        point3(4, 1, 0), 1.0, make_shared<metal>(color(0.7, 0.6, 0.5), 0.0)));

    return world;
}



const int nx = 1200;
const int ny = 800;
int ns = 10;
unsigned char rgb[nx * ny * 3];
unsigned char* p = rgb;
int main() {
    auto start = std::chrono::system_clock::now();
    const auto aspect_ratio = 1.0 / 1.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    FILE* fp;
    fopen_s(&fp, "Image1.png", "wb");
    hittable_list world;

    int samples_per_pixel = 100;
    int max_depth = 50;

    point3 lookfrom;
    point3 lookat;
    vec3 vup(0, 1, 0);
    auto vfov = 40.0;
    auto aperture = 0.0;
    auto dist_to_focus = 10.0;
    color background(0, 0, 0);
    world = random_scene();
    lookfrom = point3(13, 2, 3);
    lookat = point3(0, 0, 0);
    vfov = 20.0;
    background = color(0.70, 0.80, 1.00);
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s=0; s < ns; s++) {
                //calculate the position of the pixel position and allocate the ray 
                float u = float(i + random_double()) / float(nx);
                float v = float(j + random_double()) / float(ny);
                ray r = cam.get_ray(u, v);
                col += ray_color(r, background, world, max_depth);
            }
            col /= float(ns);
            col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
            *p++ = int(255.99*col[0]);

            *p++ = int(255.99*col[1]);
            *p++ = int(255.99 * col[2]);
        }
    }
    svpng(fp, nx, ny, rgb, 0);
    fclose(fp);
    auto end = chrono::system_clock::now();
    auto time = end - start;
    cout << duration<double, milli>(time).count() / 1000 << "s" << endl;
    return 0;
}

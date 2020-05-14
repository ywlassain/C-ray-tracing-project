#ifndef CAMERAH
#define CAMERAH


#include "ray.h"
#include "random.h"

vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2.0*vec3(random_double(),random_double(),0) - vec3(1,1,0);
    } while (dot(p,p) >= 1.0);
    return p;
}


class camera {
public:
    camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist,
        double t0 = 0, double t1 = 0) {
        origin = lookfrom;
        lens_radius = aperture / 2;
        time0 = t0;
        time1 = t1;
        auto theta = degrees_to_radians(vfov);
        auto half_height = tan(theta / 2);
        auto half_width = aspect_ratio * half_height;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
        horizontal = 2 * half_width * focus_dist * u;
        vertical = 2 * half_height * focus_dist * v;
    }
    ray get_ray(double s, double t)
    {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, random_double(time0, time1));
    }
private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
    double time0, time1;
    double degrees_to_radians(double angle)
    {
        return angle * M_PI / 180;
    }
};

#endif

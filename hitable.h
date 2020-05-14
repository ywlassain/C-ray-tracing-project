#ifndef HITABLEH
#define HITABLEH
#include"random.h"

#include "./project2/aabb.h"
#include<cmath>

class material;

void get_sphere_uv(const point3& p, double& u, double& v) {
    auto phi = atan2(p.z(), p.x());
    auto theta = asin(p.y());
    u = 1 - (phi + M_PI) / (2 * M_PI);
    v = (theta + M_PI / 2) / M_PI;
}


struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    double u;
    double v;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};


class  hittable {
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
    //virtual bool bounding_box(double t0, double t1, aabb& output_box) const = 0;
};


#endif
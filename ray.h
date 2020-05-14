#ifndef RAYH
#define RAYH
//==================================================================================================
// Written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is distributed
// without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication along
// with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==================================================================================================

#include "vec3.h"


class ray
{
public:
    ray() {}
    //For the motion blur IO need to settle the random time to create the ray when the shutter is open
    ray(const point3& origin, const vec3& direction, double time = 0.0)
        :orig(origin), dir(direction), tm(time) {}
    vec3 origin() const { return orig; }
    vec3 direction() const { return dir; }
    double time()const { return tm; }
    vec3 point_at_parameter(float t) const { return orig + t*dir; }
    point3 at(double t)const {
        return orig + t * dir;
    }
    point3 orig;
    vec3 dir;
   double tm;
};


#endif

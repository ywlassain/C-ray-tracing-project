#ifndef RANDOMH
#define RANDOMH

#include <cstdlib>
#include<random>
#include<iostream>
#include<limits>
#include<memory>
#include<cmath>
using namespace std;
using std::shared_ptr;
using std::make_shared;
using std::sqrt;
inline double random_double()
{
    return rand() / (RAND_MAX + 1.0f);
}
const double infinity = numeric_limits<double>::infinity();
//this is the function for all the random function
inline double random_double(double min,double max) {
    return min + (max - min) * random_double();
}
inline int random_int(int min, int max)
{
    return static_cast<int>(random_double(min, max + 1));
}
inline double clmap(double x, double min, double max)
{
    if (x < min)return min;
    if (x > max)return max;
    return x;
}
inline double degress_to_radians(double degrees)
{
    return degrees * M_PI / 180.0f;
}

#endif

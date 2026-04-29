#ifndef RTRACE_H
#define RTRACE_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;

const double infinity = std::numeric_limits<double>::infinity();
const double pi         = 3.1415926535897932385;
const double two_pi     = 6.2831853071795864769;
const double pi_d_180   = 0.017453292519943295;
const double pi_d_2     = 1.5707963267948966;
const double pi_d_4     = 0.7853981633974483;

inline double degree_to_radian(double degrees){
    return degrees * pi_d_180;
}

inline double random_double(){ //returns a random real in [0,1)
    return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max){
    return min + (max-min)*random_double();
}

inline int random_int(int min, int max){
    return int(random_double(min, max+1));
}

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif
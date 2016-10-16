#ifndef RAY_H
#define RAY_H

#include "Vec3f.h"
class ray
{
public:
    Vec3f start;
    Vec3f direct;
    ray(){}
    ray(Vec3f _start,Vec3f _direct):start(_start),direct(_direct){}
};

#endif // RAY_H

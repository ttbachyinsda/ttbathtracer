#ifndef ZHUANG_H
#define ZHUANG_H
#include "ray_math_algo.h"
#include "Vec3f.h"
using namespace std;
class zhuang
{
public:
    bool isfront;
    Vec3f N,wz;
    double dist;
    zhuang()
    {
        isfront=0; dist=0;
    }
    zhuang(bool _isfront,Vec3f _N,Vec3f _wz, double _dist):isfront(_isfront),N(_N),wz(_wz),dist(_dist){}
};
#endif // ZHUANG_H

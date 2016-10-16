#ifndef LIGHT_H
#define LIGHT_H
#include "color.h"
#include "ray.h"
#include "Vec3f.h"
#include "ray_math_algo.h"
#include "object.h"
#include <vector>
class light
{
public:
    int bianhao;
    double power;
    color nowcolor;
    int type;
    Vec3f wz;
    light* next;
    light()
    {
        bianhao=rand();
        type=-1;
        next=0;
        power=1;
    }

    virtual void readfromfile(string filename)=0;
    virtual bool pengzhuang(ray nowray,double &nowdist)=0;
    virtual double calshade(Vec3f point,object* head,int cttime)=0;
};

#endif // LIGHT_H

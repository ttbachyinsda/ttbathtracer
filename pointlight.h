#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "light.h"
#include <fstream>
class pointlight:public light
{
public:
    void readfromfile(string filename)
    {
        ifstream input(filename);
        type=0;
        bianhao=rand();
        nowcolor.reader(input);
        input>>wz.x>>wz.y>>wz.z;
        next=0;
    }

    bool pengzhuang(ray nowray,double &nowdist)
    {
        return false;
    }

    double calshade(Vec3f point,object* head,int cttime)
    {
        Vec3f direct=wz-point;
        double dist=direct.mole();
        for (object*now=head;now;now=now->next)
        {
            ray nowray(point,direct);
            zhuang info;
            if (now->pengzhuang(nowray,info))
                if (info.dist<dist)
                    return 0;
        }
        return 1;
    }
};

#endif // POINTLIGHT_H

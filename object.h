#ifndef OBJECT_H
#define OBJECT_H
using namespace std;
#include "myimg.h"
#include "material.h"
#include "ray.h"
#include "zhuang.h"
class object
{
public:
    int bianhao;
    bool absorb;
    material *mt;
    object* next;
    object()
    {
        bianhao=rand();
        mt=0;
        next=0;
        absorb=false;
    }
    virtual void readfromfile(string filename)=0;
    virtual bool pengzhuang(ray nowray,zhuang &info)=0;
    virtual color getcolor(zhuang info)=0;
    virtual void outputdata()=0;
};

#endif // OBJECT_H

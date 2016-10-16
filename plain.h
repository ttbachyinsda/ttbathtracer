#ifndef PLAIN_H
#define PLAIN_H
#include "object.h"
class plain:public object
{
public:
    Vec3f N;
    double dist;
    Vec3f xzhou,yzhou;
    void readfromfile(string filename)
    {
        ifstream input(filename);
        input>>xzhou.x>>xzhou.y>>xzhou.z;
        input>>yzhou.x>>yzhou.y>>yzhou.z;
        input>>N.x>>N.y>>N.z>>dist;
        mt=new material();
        mt->readfromfile(input);
        next=0;
    }
    bool pengzhuang(ray nowray, zhuang &info)
    {
        nowray.direct.Normalize();
        N.Normalize();
        double costhi=N.dot(nowray.direct);
        double l=(N*dist-nowray.start).dot(N)/costhi;
        if (fabs(costhi)<EPS || l<EPS) return false;
        info.dist=l;
        if (costhi<0) info.isfront=true; else info.isfront=false;
        info.wz=nowray.start+nowray.direct*info.dist;
        info.N=(info.isfront)?N:-N;
        return true;
    }
    color getcolor(zhuang info)
    {
        double u=info.wz.dot(xzhou)/xzhou.sqr();
        double v=info.wz.dot(yzhou)/yzhou.sqr();
        u=u-floor(u);
        v=v-floor(v);
        return mt->getcolor(u*mt->background->height,v*mt->background->width);
    }
    void outputdata()
    {

    }
};

#endif // PLAIN_H

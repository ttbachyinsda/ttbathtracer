#ifndef BALL_H
#define BALL_H
#include "object.h"
using namespace std;
class ball:public object
{
public:
    Vec3f center;
    double r;
    Vec3f xzhou,yzhou;
    color getcolor(zhuang info)
    {
        Vec3f pointvec=info.wz-center;
        pointvec.Normalize();
        xzhou.Normalize();
        yzhou.Normalize();
        double xfenliang=pointvec.dot(xzhou);
        double yfenliang=pointvec.dot(yzhou);
        double tu=asin(xfenliang)/PI+0.5;
        double tv=asin(yfenliang)/PI+0.5;
        tu=fabs(tu); tv=fabs(tv);
        tu=1-tu; tv=1-tv;
        double h=mt->background->height,w=mt->background->width;
        return mt->getcolor(tu*h,tv*w);
    }
    bool pengzhuang(ray nowray, zhuang &info)
    {
        nowray.direct.Normalize();
        Vec3f l=nowray.start-center;
        double tp=-(l.dot(nowray.direct));
        double sqrd=tp*tp-l.sqr()+r*r;
        if (sqrd>EPS)
        {
            sqrd=sqrt(sqrd);
            double dis1=tp-sqrd,dis2=tp+sqrd;
            if (dis2<EPS) return false;
            else
            {
                if (dis1>EPS)
                {
                    info.dist=dis1;
                    info.isfront=true;
                } else
                {
                    info.dist=dis2;
                    info.isfront=false;
                }
            }
        } else return false;
        info.wz=nowray.start+nowray.direct*info.dist;
        info.N=(info.wz-center);
        info.N=info.N.getnormalize();
        if (!info.isfront) info.N=-info.N;
        return true;
    }
    void readfromfile(string filename)
    {
        bianhao=rand();
        ifstream input(filename);
        xzhou=Vec3f(1,0,-1);
        yzhou=Vec3f(1,0,1);
        input>>center.x>>center.y>>center.z>>r;
        mt=new material();
        mt->readfromfile(input);
        next=0;
    }
    void outputdata()
    {

    }
};

#endif // BALL_H

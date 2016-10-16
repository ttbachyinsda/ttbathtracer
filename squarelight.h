#ifndef SQUARELIGHT_H
#define SQUARELIGHT_H
#include "light.h"
#include <fstream>
class squarelight:public light
{
public:
    Vec3f vecx,vecy;
    void readfromfile(string filename)
    {
        ifstream input(filename);
        type=1;
        bianhao=rand();
        nowcolor.reader(input);
        input>>wz.x>>wz.y>>wz.z;
        input>>vecx.x>>vecx.y>>vecx.z;
        input>>vecy.x>>vecy.y>>vecy.z;
        input>>power;
        next=0;
    }

    bool pengzhuang(ray nowray,double &nowdist)
    {
        nowray.direct.Normalize();
        Vec3f N=(vecx*vecy).getnormalize();
        double costhi=N.dot(nowray.direct);
        double l=(N*wz.dot(N)-nowray.start).dot(N)/costhi;
        if (fabs(costhi)<EPS || l<EPS) return false;
        Vec3f temp=nowray.start+nowray.direct*l-wz;
        if (fabs(vecx.dot(temp))>vecx.sqr()) return false;
        if (fabs(vecy.dot(temp))>vecy.sqr()) return false;
        nowdist=l;
        return true;
    }

    double calshade(Vec3f point,object* head,int cttime)
    {
        double shade=0;
        for (int i=0;i<4;i++)
        {
            for (int j=0;j<4;j++)
            for (int k=0;k<cttime;k++)
            {
                if (i==1 && j==0 && k==3)
                {
                    //cout<<i<<' '<<j<<' '<<k<<endl;
                }
                Vec3f lightwz = wz + vecx*rand_at(-2+i,-1+i)/2 + vecy*rand_at(-2+j,-1+j)/2;
                Vec3f direct=lightwz-point;
                double dist=direct.mole();
                for (object*now=head;now;now=now->next)
                {
                    ray nowray(point,direct);
                    zhuang info;
                    if (now->pengzhuang(nowray,info))
                        if (info.dist<dist)
                            shade=shade+1;
                }
            }
        }
        return 1-shade/(cttime*16.0);
    }
};
#endif // SQUARELIGHT_H

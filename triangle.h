#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "object.h"
class triangle:public object
{
public:
    Vec3f p0,p1,p2;
    Vec3f N;
    Vec3f trueNp0,trueNp1,trueNp2;
    Vec3f e1,e2;
    double nv,nu,nd;
    double bnu,bnv,cnu,cnv;
    double mins[3],maxs[3];
    int maxt;
    int xrtime;
    triangle* trianglenext;
    void readfromfile(string filename)
    {
        ifstream input(filename);
        p0.reader(input);
        p1.reader(input);
        p2.reader(input);
        mt=new material();
        mt->readfromfile(input);
        bianhao=rand();
        next=0;
        trianglenext=0;
        gash();
    }
    void gash()
    {
        xrtime=0;
        mins[0]=p0.x; if (p1.x<mins[0]) mins[0]=p1.x; if (p2.x<mins[0]) mins[0]=p2.x;
        mins[1]=p0.y; if (p1.y<mins[1]) mins[1]=p1.y; if (p2.y<mins[1]) mins[1]=p2.y;
        mins[2]=p0.z; if (p1.z<mins[2]) mins[2]=p1.z; if (p2.z<mins[2]) mins[2]=p2.z;
        maxs[0]=p0.x; if (p1.x>maxs[0]) maxs[0]=p1.x; if (p2.x>maxs[0]) maxs[0]=p2.x;
        maxs[1]=p0.y; if (p1.y>maxs[1]) maxs[1]=p1.y; if (p2.y>maxs[1]) maxs[1]=p2.y;
        maxs[2]=p0.z; if (p1.z>maxs[2]) maxs[2]=p1.z; if (p2.z>maxs[2]) maxs[2]=p2.z;
        e1=p1-p0;
        e2=p2-p0;
        nv=0; nu=0; nd=0;
        bnu=0; bnv=0; cnu=0; cnv=0; maxt=0;
        N = e2*e1;
        if(N.iszero()){
            N=Vec3f(0,0,1);
            //cout<<"canerror"<<endl;
            return;
        }
        double maxfx=fabs(N.x); maxt=0;
        if (fabs(N.y)>maxfx) maxfx=fabs(N.y),maxt=1;
        if (fabs(N.z)>maxfx) maxfx=fabs(N.z),maxt=2;
        int u=(maxt+1)%3,v=(maxt+2)%3;
        double d=1.0/N[maxt];
        nu=N[u]*d;
        nv=N[v]*d;
        nd=N.dot(p0)*d;
        double c = 1.0/(e2[u]*e1[v]-e2[v]*e1[u]);
        bnu=e2[u]*c;
        bnv=-e2[v]*c;
        cnu=e1[v]*c;
        cnv=-e1[u]*c;
        N.Normalize();
    }

    bool pengzhuang(ray nowray, zhuang &info)
    {
        //return pengzhuang2(nowray,info);
        nowray.direct.Normalize();
        int u=(maxt+1)%3,v=(maxt+2)%3;
        double lnd=1.0f/(nowray.direct[maxt]+nu*nowray.direct[u]+nv*nowray.direct[v]);
        if (lnd>1e+8) return false;
        double l=(nd-nowray.start[maxt]-nu*nowray.start[u]-nv*nowray.start[v])*lnd;
        if (l<EPS) return false;
        double hu=nowray.start[u]+l*nowray.direct[u]-p0[u];
        double hv=nowray.start[v]+l*nowray.direct[v]-p0[v];
        double x=hv*bnu+hu*bnv;
        if (x<0) return false;
        double y=hv*cnv+hu*cnu;
        if (y<0) return false;
        if (x+y>1) return false;
        info.wz=nowray.start+nowray.direct*l;
        Vec3f NewN=(trueNp0*(1-x-y)+trueNp1*x+trueNp2*y).getnormalize();
        if (NewN.dot(nowray.direct)<0) info.isfront=1;
         else info.isfront=0;
        info.N=(info.isfront)?NewN:-NewN;
        info.dist=l;
        xrtime=xrtime+1;
        return true;
    }

    color getcolor(zhuang info)
    {
        double u=info.wz.dot(e1)/e1.sqr();
        double v=info.wz.dot(e2)/e2.sqr();
        u=u-floor(u);
        v=v-floor(v);
        return mt->getcolor(u*mt->background->height,v*mt->background->width);
    }
    bool pdin(Vec3f point)
    {
        if ((p0-point).mole()<EPS) return true;
        if ((p1-point).mole()<EPS) return true;
        if ((p2-point).mole()<EPS) return true;
        Vec3f f1=p0-point;
        Vec3f f2=p1-point;
        Vec3f f3=p2-point;
        if (fabs((f1.getnormalize().dot(f2.getnormalize()))+1)<EPS) return true;
        if (fabs((f2.getnormalize().dot(f3.getnormalize()))+1)<EPS) return true;
        if (fabs((f3.getnormalize().dot(f1.getnormalize()))+1)<EPS) return true;
        f1.Normalize();
        f2.Normalize();
        f3.Normalize();
        double st=acos(f1.dot(f2))+acos(f2.dot(f3))+acos(f3.dot(f1));
        if (fabs(st-2*PI)<EPS) return true; else return false;
    }
    void outputdata()
    {

    }
};
#endif // TRIANGLE_H

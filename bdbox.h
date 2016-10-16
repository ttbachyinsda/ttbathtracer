#ifndef BDBOX_H
#define BDBOX_H
#include "object.h"
#include "triangle.h"
using namespace std;
class bdbox:public object
{
public:
    Vec3f point1,point2;
    color getcolor(zhuang info)
    {
        return color(1,1,1);
    }
    bool pdin(Vec3f point)
    {
        for (int i=0;i<3;i++)
        {
            if (point1._p[i]>point2._p[i])
            {
                cout<<"error22"<<endl;
            }
            if (point._p[i]<point1._p[i]-EPS) return false;
            if (point._p[i]>point2._p[i]+EPS) return false;
        }
        return true;
    }
    double getsize()
    {
        double sizer=fabs((point2.x-point1.x)*(point2.y-point1.y)*(point2.z-point1.z));
        return sizer;
    }
    double getmianji()
    {
        double s1=point2.x-point1.x;
        double s2=point2.y-point1.y;
        double s3=point2.z-point1.z;
        return (s1*s2+s2*s3+s3*s1)*2;
    }

    void repair()
    {
        if (point2.x-point1.x<0.025)
        {
            point2.x += 0.025;
            point1.x -= 0.025;
        }

        if (point2.y-point1.y<0.025)
        {
            point2.y += 0.025;
            point1.y -= 0.025;
        }
        if (point2.z-point1.z<0.025)
        {
            point2.z += 0.025;
            point1.z -= 0.025;
        }
    }

    void createbox(vector<triangle*> contain,int size)
    {
        point1=Vec3f(1e+35,1e+35,1e+35);
        point2=Vec3f(-1e+35,-1e+35,-1e+35);
        for (int i=0;i<size;i++)
        {
            point1.x=min(contain[i]->p0.x,contain[i]->p1.x,contain[i]->p2.x,point1.x);
            point1.y=min(contain[i]->p0.y,contain[i]->p1.y,contain[i]->p2.y,point1.y);
            point1.z=min(contain[i]->p0.z,contain[i]->p1.z,contain[i]->p2.z,point1.z);
            point2.x=max(contain[i]->p0.x,contain[i]->p1.x,contain[i]->p2.x,point2.x);
            point2.y=max(contain[i]->p0.y,contain[i]->p1.y,contain[i]->p2.y,point2.y);
            point2.z=max(contain[i]->p0.z,contain[i]->p1.z,contain[i]->p2.z,point2.z);
        }
        repair();
    }

    double quickpengzhuang(ray nowray)
    {
        nowray.direct.Normalize();
      if (!pdin(nowray.start))
      {
        double mindist=-1,mint=-1;
        for (int i=0;i<3;i++)
        {
            double disti=-1;
            if (nowray.direct._p[i]>=EPS)
                disti=(point1[i]-nowray.start._p[i])/nowray.direct._p[i];
            else if (nowray.direct._p[i]<=-EPS)
                disti=(point2[i]-nowray.start._p[i])/nowray.direct._p[i];
            if (disti>=EPS)
            {
                Vec3f jd=nowray.start+nowray.direct*disti;
                if (pdin(jd))
                {
                    if (mint==-1 || disti<mindist)
                    {
                        mindist=disti;
                        mint=i;
                    }
                }
            }
        }
        return mindist;
      }
      else
      {
          double mindist=-1,mint=-1;
          for (int i=0;i<3;i++)
          {
              double disti=-1;
              if (nowray.direct._p[i]>=EPS)
                  disti=(point2[i]-nowray.start._p[i])/nowray.direct._p[i];
              else if (nowray.direct._p[i]<=-EPS)
                  disti=(point1[i]-nowray.start._p[i])/nowray.direct._p[i];
              if (disti>=EPS)
              {
                  Vec3f jd=nowray.start+nowray.direct*disti;
                  if (pdin(jd))
                  {
                      if (mint==-1 || disti<mindist)
                      {
                          mindist=disti;
                          mint=i;
                      }
                  }
              }
          }
          return mindist;
      }
    }

    bool pengzhuang(ray nowray, zhuang &info)
    {
      if (!pdin(nowray.start))
      {
        nowray.direct.Normalize();
        double mindist=-1,mint=-1; Vec3f wz,N;
        for (int i=0;i<3;i++)
        {
            double disti=-1; Vec3f tN;
            if (nowray.direct._p[i]>=EPS)
                disti=(point1[i]-nowray.start._p[i])/nowray.direct._p[i],tN=Vec3f((i==0)?1:0,(i==1)?1:0,(i==2)?1:0);
            else if (nowray.direct._p[i]<=-EPS)
                disti=(point2[i]-nowray.start._p[i])/nowray.direct._p[i],tN=Vec3f((i==0)?-1:0,(i==1)?-1:0,(i==2)?-1:0);
            if (disti>=EPS)
            {
                Vec3f jd=nowray.start+nowray.direct*disti;
                if (pdin(jd))
                {
                    if (mint==-1 || disti<mindist)
                    {
                        mindist=disti;
                        mint=i;
                        wz=jd;
                        N=tN;
                    }
                }
            }
        }
        if (mint==-1) return false;
        else
        {
            info.wz=wz;
            info.N=N;
            info.dist=mindist;
            info.isfront=true;
            return true;
        }
      }
      else
      {
          nowray.direct.Normalize();
          double mindist=-1,mint=-1; Vec3f wz,N;
          for (int i=0;i<3;i++)
          {
              double disti=-1; Vec3f tN;
              if (nowray.direct._p[i]<=EPS)
                  disti=(point1[i]-nowray.start._p[i])/nowray.direct._p[i],tN=Vec3f((i==0)?1:0,(i==1)?1:0,(i==2)?1:0);
              else if (nowray.direct._p[i]>=-EPS)
                  disti=(point2[i]-nowray.start._p[i])/nowray.direct._p[i],tN=Vec3f((i==0)?-1:0,(i==1)?-1:0,(i==2)?-1:0);
              if (disti>=EPS)
              {
                  Vec3f jd=nowray.start+nowray.direct*disti;
                  if (pdin(jd))
                  {
                      if (mint==-1 || disti<mindist)
                      {
                          mindist=disti;
                          mint=i;
                          wz=jd;
                          N=tN;
                      }
                  }
              }
          }
          if (mint==-1) return false;
          else
          {
              info.wz=wz;
              info.N=N;
              info.dist=mindist;
              info.isfront=false;
              return true;
          }
      }
    }
    void readfromfile(string filename)
    {
        bianhao=rand();
        ifstream input(filename);
        point1.reader(input);
        point2.reader(input);
        mt=new material();
        mt->readfromfile(input);
        next=0;
    }
    void outputdata()
    {

    }
};
#endif // BDBOX_H

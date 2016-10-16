#ifndef PATHTRACING_H
#define PATHTRACING_H
#include "scene.h"
#include <omp.h>
#define MAX_DEP 5
class pathtracer
{
public:
    scene nowscene;
    inline Vec3f importancesample(Vec3f nowdirect,double n)
    {
        double z,phi,theta;
        z=rand_at(0,1);
        phi=rand_at(0,1)*2*PI;
        theta=(n==1?acos(sqrt(z)):acos(pow(z,1/(n+1))));
        double sintheta=sin(theta);
        Vec3f sample(sintheta*cos(phi),sintheta*sin(phi),z);
        Vec3f t(rand_at(0,1),rand_at(0,1),rand_at(0,1));
        Vec3f u=t*nowdirect; u.Normalize();
        Vec3f v=nowdirect*u;
        Vec3f rot1=Vec3f(u.x,v.x,nowdirect.x);
        Vec3f rot2=Vec3f(u.y,v.y,nowdirect.y);
        Vec3f rot3=Vec3f(u.z,v.z,nowdirect.z);
        Vec3f res=Vec3f(rot1.dot(sample),rot2.dot(sample),rot3.dot(sample));
        if (res.dot(nowdirect)<0) res=-res;
        return res;

    }

    inline double fanshesurvive(object *rightobj)
    {
        double spec=rightobj->mt->pjingmianmanfan;
        double diff=rightobj->mt->pmanfan;
        double t=spec/(spec+diff);
        double survivep=1.0/t;
        double p=rand_at(0,1);
        if (p>t) return survivep; else return -survivep;
    }
    inline double normalsurvive(double refl)
    {
        double survivep=1.0/refl;
        if (rand_at(0,1)<refl) return survivep; else return -survivep;
    }
    inline double schlickcalc(double nin,double nout,Vec3f indirect,Vec3f outdirect,zhuang info)
    {
        double nbigger=max(nin,nout),nsmaller=min(nin,nout);
        double r0=sqr((nbigger-nsmaller)/(nbigger+nsmaller));
        double c;
        indirect.Normalize(); outdirect.Normalize();
        info.N.Normalize();
        Vec3f trueN=info.isfront?info.N:-info.N;
        bool isfront=(trueN.dot(indirect)<0);
        if (isfront)
            c=1-((-indirect).dot(trueN));
        else
            c=1-(outdirect.dot(trueN));
        //cout<<c<<endl;
        double schlick=r0+(1-r0)*c*c*c*c*c;
        if (schlick>1 || schlick<0)
        {
            cout<<"error4"<<endl;
        }
        return schlick;
    }

    inline color calspec(object * rightobj,zhuang info,ray inray,int dep)
    {
        color nowcolor;
        if (rightobj->mt->background)
            nowcolor=rightobj->mt->mcolor*rightobj->getcolor(info);
        else
            nowcolor=rightobj->mt->mcolor;
        Vec3f reflect=(inray.direct.reflect(info.N)).getnormalize();
        Vec3f raydir=importancesample(reflect,20);
        inray.direct=raydir;
        inray.start=info.wz;
        inray.direct.Normalize();
        color ret= pathtracing(inray,dep+1);
        return ret*fabs(raydir.dot(info.N))*nowcolor;
    }

    inline color calmanfanshe(object* rightobj,zhuang info,int dep)
    {
        color nowcolor;
        if (rightobj->mt->background)
            nowcolor=rightobj->mt->mcolor*rightobj->getcolor(info);
        else
            nowcolor=rightobj->mt->mcolor;
        Vec3f naive=importancesample(info.N,1);
        ray newray;
        newray.direct=naive;
        newray.start=info.wz;
        newray.direct.Normalize();
        color ret= pathtracing(newray,dep+1)*nowcolor;
        return ret;
    }
    inline color calfanshe(object* rightobj,zhuang info,ray inray,int dep)
    {
        color nowcolor=rightobj->mt->mcolor;
        inray.direct=inray.direct.reflect(info.N);
        inray.start=info.wz;
        inray.direct.Normalize();
        if (rightobj->mt->rjingmianmanfan<EPS)
            return pathtracing(inray,dep+1)*nowcolor;
        Vec3f dx(1,0,0);
        dx=inray.direct*dx;
        if (dx.iszero()) dx=Vec3f(1,0,0);
        Vec3f dy=inray.direct*dx;
        double fx=1,fy=1;
        while (sqr(fx)+sqr(fy)>1)
        {
            fx=rand_at(-1,1);
            fy=rand_at(-1,1);
        }
        fx *= rightobj->mt->rjingmianmanfan;
        fy *= rightobj->mt->rjingmianmanfan;
        ray newray=inray;
        newray.direct=newray.direct+dx*fx+dy*fy;
        color ret = pathtracing(newray,dep+1)*nowcolor;
        return ret;
    }
    inline bool calzheshe(object* rightobj,zhuang info,ray inray,ray &newray,double &schlick)
    {
        double n0=rightobj->mt->zheshelv,n1=1;
        Vec3f trueN=info.isfront?info.N:-info.N;
        bool isfront=(trueN.dot(inray.direct)<0);
        if (isfront) swap(n0,n1);
        if (isfront!=info.isfront)
        {
            //cout<<isfront<<' '<<info.isfront<<endl;
            cout<<"perror"<<endl;
        }
        inray.direct.Normalize();
        Vec3f newdirect;
        bool iszheshe=inray.direct.canzheshe(info.N,n0/n1,newdirect);
        if (!iszheshe) return false;
        schlick=schlickcalc(n0,n1,inray.direct,newdirect,info);
        if (iszheshe)
        {
            newray=inray;
            newray.direct=newdirect;
            newray.start=info.wz;
        }
        return iszheshe;
    }
    inline color zhesher(object* rightobj,zhuang info,ray inray,int dep)
    {
        color absor=rightobj->mt->acolor*info.dist;
        absor.r=exp(-absor.r);
        absor.g=exp(-absor.g);
        absor.b=exp(-absor.b);
        color result=absor*pathtracing(inray,dep+1);
        if (result.r<0 || result.g<0 || result.b<0)
        {
            cout<<"error3"<<endl;
        }
        return result;
    }

    inline color pathtracing(ray nowray,int dep)
    {
        color bgm=nowscene.background;
        if (dep>200) return bgm;
        color ret=bgm;
        zhuang infoobj; double infolight;
        //cout<<"rc1"<<endl;
        object* nearobj=nowscene.findnearobj(nowray,infoobj);
        light* nearlight=nowscene.findnearlight(nowray,infolight);
        if (nearlight && (!nearobj || infolight<infoobj.dist))
        {
            ret = nearlight->nowcolor*nearlight->power;
return ret;
        }
        else if (nearobj)
        {
            color nowcolor=(nearobj->mt->background==0)?nearobj->mt->mcolor:nearobj->mt->mcolor*nearobj->getcolor(infoobj);
            double cs=max(nowcolor.r,nowcolor.g,nowcolor.b);
            double survive=1.0;
            if (dep>MAX_DEP)
            {
                survive=normalsurvive(cs);
                if (survive<0) return bgm;
            }
            double c[3];
            c[0]=nearobj->mt->pmanfan;
            c[1]=nearobj->mt->pjingmianmanfan+c[0];
            c[2]=nearobj->mt->pfan+c[1];
            double k=rand_at(0,1);
            if (k<=c[0])
            {
                color sqe=calmanfanshe(nearobj,infoobj,dep);
                ret = sqe*survive;
                if (ret.r<0 || ret.b<0 || ret.g<0)
                {
                    cout<<"error"<<endl;
                }
            }
            else if (k<=c[1])
            {
                double survivemanfan=fanshesurvive(nearobj);
                if (survivemanfan>0)
                    ret = calmanfanshe(nearobj,infoobj,dep)*survive*(1.0/(1-1.0/survivemanfan));
                else
                    ret = calspec(nearobj,infoobj,nowray,dep)*survive*(-survivemanfan);
            }
            else if (k<=c[2])
                ret = calfanshe(nearobj,infoobj,nowray,dep)*survive;
            else
            {
                bool iszheshe; double schlick; ray newray;
                double survivezheshe;
                iszheshe=calzheshe(nearobj,infoobj,nowray,newray,schlick);
                if (!iszheshe)
                {
                    ret = calfanshe(nearobj,infoobj,nowray,dep)*survive;
                }
                else
                {
                    /*
                  if (dep<5)
                  {
                    ret = zhesher(nearobj,infoobj,newray,dep,xishu)*survive*(1-schlick);
                    ret = ret + calfanshe(nearobj,infoobj,nowray,dep,xishu)*survive*schlick;
                  }else
                  {}
                  */
                    double c=0.25+0.5*schlick;
                      survivezheshe=normalsurvive(c);
                      if (survivezheshe<0)
                      {
                          ret = zhesher(nearobj,infoobj,newray,dep)*(survive*(1.0/(1-1.0/(-survivezheshe)))*(1-schlick));
                      }
                      else
                          ret = calfanshe(nearobj,infoobj,nowray,dep)*(survive/c*schlick);
                }
                if (ret.r<0 || ret.g<0 || ret.b<0)
                {
                    cout<<"eee"<<endl;
                }

            }
        }
        return ret;
    }
    void run(int samples,bool infinite)
    {
        int H=nowscene.nowcamera->height;
        int W=nowscene.nowcamera->width;
    double pi[4]={0.25,-0.25,0.25,-0.25};
    double pj[4]={0.25,0.25,-0.25,-0.25};
    int nowsamples=0;
    color *ftotal= new color[H*W];
    while (true)
    {
        nowsamples += samples;
        cout<<"now samples:"<<nowsamples<<endl;
    #pragma omp parallel for schedule(dynamic)
        for (int i=0;i<H;i++)
        {
            if (i%50==0) safeprint(i,H);
            Vec3f csfx;
            ray csray;
            for (int j=0;j<W;j++)
            {
                color total(0,0,0);
                if (!nowscene.nowcamera->focuson)
                {
                for (int k=0;k<4;k++)
                {
                    double newi=i+pi[k];
                    double newj=j+pj[k];
                    csfx=nowscene.nowcamera->shechu(newi,newj);
                    csray.start=nowscene.nowcamera->wz;
                    csray.direct=csfx;
                    csray.direct.Normalize();
                    for (int l=0;l<samples;l++)
                    {
                        color temp=pathtracing(csray,0);
                        total=total+temp;
                    }
                }
                }
                else
                {
                    for (int k=0;k<4;k++)
                    {
                        double newi=i+pi[k];
                        double newj=j+pj[k];
                        csray=nowscene.nowcamera->focusshechu(newi,newj);
                        csray.direct.Normalize();
                        for (int l=0;l<samples;l++)
                        {
                            color temp=pathtracing(csray,0);
                            total=total+temp;
                        }
                    }
                }
                ftotal[i*W+j] = ftotal[i*W+j]+total;

            }
        }
        for (int i=0;i<H;i++)
            for (int j=0;j<W;j++)
            {
                color total=ftotal[i*W+j]/(4*nowsamples);
                total.Repaircolor();
                nowscene.nowcamera->nowimage->drawpixel(i,j,total);
            }
        if (!infinite) break;
        nowscene.nowcamera->nowimage->rotate();
        nowscene.nowcamera->output(filepath("process.bmp"));
        swap(nowscene.nowcamera->nowimage->height,nowscene.nowcamera->nowimage->width);
        ofstream egs(filepath("message.txt"));
        egs<<nowsamples<<' '<<H<<' '<<W<<endl;
        for (int i=0;i<H;i++)
            for (int j=0;j<W;j++)
            {
                egs<<ftotal[i*W+j].r<<' '<<ftotal[i*W+j].g<<' '<<ftotal[i*W+j].b<<endl;
            }
        egs.close();
    }
    }
};
#endif // PATHTRACING_H

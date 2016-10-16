#ifndef RAYTRACING_H
#define RAYTRACING_H
#include "scene.h"
#include <omp.h>
#define MAX_MANFANDEP 2
#define MAX_TOTALDEP 10
class raytracer
{
public:
    scene nowscene;
    color calmanfanshe(object* rightobj,zhuang info,int &nowhash)
    {
        color nowcolor;
        if (rightobj->mt->background)
            nowcolor=rightobj->mt->mcolor*rightobj->getcolor(info);
        else
            nowcolor=rightobj->mt->mcolor;
        color ret=nowcolor*nowscene.background*rightobj->mt->pmanfan;
        for (light*rightlight=nowscene.nowlight;rightlight;rightlight=rightlight->next)
        {
            double shade=rightlight->calshade(info.wz,nowscene.nowobj,nowscene.nowcamera->shadecttime);
            if (shade<EPS) continue;
            Vec3f R=rightlight->wz-info.wz;
            R.Normalize();
            double dot=R.dot(info.N);
            if (dot>EPS)
            {
                if (nowhash && rightlight->type==0)
                {
                    nowhash = (nowhash+rightlight->bianhao) ;
                }
                if (rightobj->mt->pmanfan>EPS)
                {
                    double manfan=rightobj->mt->pmanfan*dot*shade;
                    ret = ret+nowcolor*rightlight->nowcolor*manfan;
                }
                if (rightobj->mt->pjingmianmanfan>EPS)
                {
                    double jingmianmanfan=rightobj->mt->pjingmianmanfan*pow(dot,20)*shade;
                    ret = ret+nowcolor*rightlight->nowcolor*jingmianmanfan;
                }
            }
        }

        return ret;
    }
    color calfanshe(object* rightobj,zhuang info,ray inray,int dep,int &nowhash)
    {
        color nowcolor=rightobj->mt->mcolor;
        inray.direct=inray.direct.reflect(info.N);
        inray.start=info.wz;
        inray.direct.Normalize();
        if (rightobj->mt->rjingmianmanfan<EPS || dep>MAX_MANFANDEP)
            return raytracing(inray,dep+1,nowhash)*nowcolor*rightobj->mt->pfan;
        Vec3f dx(1,0,0);
        dx=inray.direct*dx;
        if (dx.iszero()) dx=Vec3f(1,0,0);
        Vec3f dy=inray.direct*dx;
        color ret(0,0,0);
        int totaltime=nowscene.nowcamera->jingmianmanfancttime*16;
        for (int i=0;i<totaltime;i++)
        {
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
            int newhash=0;
            ret =ret+ raytracing(newray,dep+MAX_MANFANDEP,newhash);
        }

        ret = ret *nowcolor*rightobj->mt->pfan/totaltime;
        return ret;
    }
    color calzheshe(object* rightobj,zhuang info,ray inray,int dep,int &nowhash)
    {
        double n=rightobj->mt->zheshelv;
        if (info.isfront) n=1/n;
        inray.direct.Normalize();
        inray.direct=inray.direct.zheshe(info.N,n);
        inray.start=info.wz;
        color rcol=raytracing(inray,dep+1,nowhash);
        color result=rcol*rightobj->mt->pzhe;
        //cout<<dep<<' '<<info.isfront<<' '; result.print();
        if (info.isfront) return rcol*rightobj->mt->pzhe;
        color absor=rightobj->mt->acolor*info.dist;
        absor.r=exp(-absor.r);
        absor.g=exp(-absor.g);
        absor.b=exp(-absor.b);
        result=result*absor;
        //cout<<dep<<' '; result.print();
        return result;
    }
    color raytracing(ray nowray,int dep,int &nowhash)
    {
        if (dep>MAX_TOTALDEP) return color(0,0,0);
        color ret;
        zhuang infoobj; double infolight;
        //cout<<"rc1"<<endl;
        object* nearobj=nowscene.findnearobj(nowray,infoobj);
        light* nearlight=nowscene.findnearlight(nowray,infolight);
        if (nearlight && (!nearobj || infolight<infoobj.dist))
        {
            nowhash=(nowhash+nearlight->bianhao) ;
            ret = ret+nearlight->nowcolor;
        }
        if (nearobj)
        {
            nowhash=(nowhash+nearobj->bianhao) ;

            if (nearobj->mt->pmanfan>EPS || nearobj->mt->pjingmianmanfan>EPS)
                ret = ret+calmanfanshe(nearobj,infoobj,nowhash);

            if (nearobj->mt->pfan>EPS)
                ret = ret+calfanshe(nearobj,infoobj,nowray,dep,nowhash);

            if (nearobj->mt->pzhe>EPS)
                ret = ret+calzheshe(nearobj,infoobj,nowray,dep,nowhash);

        }
        if (!nearlight && !nearobj)
            ret=nowscene.background;
        nowhash=nowhash*5;
        ret.Repaircolor();
        //cout<<"rc2"<<endl;
        return ret;
    }
    void run()
    {
        int H=nowscene.nowcamera->height;
        int W=nowscene.nowcamera->width;
        int *hashzhi = new int[H*W];
        for (int i=0;i<H*W;i++) hashzhi[i]=0;
        int js=1;
    #pragma omp parallel for schedule(dynamic)
        for (int i=0;i<H;i++)
        {
            cout<<omp_get_num_threads()<<endl;
            //if (i>15)
            safeprint(i,H);
            Vec3f csfx;
            ray csray;
            color thiscolor;
            for (int j=0;j<W;j++)
            {
                if (!nowscene.nowcamera->focuson)
                {
                    csfx=nowscene.nowcamera->shechu(i,j);
                    csray.start=nowscene.nowcamera->wz;
                    csray.direct=csfx;
                    csray.direct.Normalize();
                    thiscolor=raytracing(csray,1,hashzhi[i*W+j]);
                    nowscene.nowcamera->nowimage->drawpixel(i,j,thiscolor);
                }
                else
                {
                    thiscolor=color(0,0,0);
                    for (int is=0;is<nowscene.nowcamera->focustime;is++)
                    {
                        csray=nowscene.nowcamera->focusshechu(i,j);
                        thiscolor=thiscolor+raytracing(csray,1,hashzhi[i*W+j]);
                    }
                    thiscolor = thiscolor / nowscene.nowcamera->focustime;
                    nowscene.nowcamera->nowimage->drawpixel(i,j,thiscolor);
                }
            }
        }

     #pragma omp parallel for schedule(dynamic)
        for (int i=0;i<H;i++)
            if (!nowscene.nowcamera->focuson)
        {
            safeprint(i,H);
            for (int j=0;j<W;j++)
            {
                bool havejuchi=false;
                ray csray;
                Vec3f csfx;
                int nowhash;
                double newi,newj;
                color thiscolor(0,0,0);
                if (j!=W-1 && hashzhi[i*W+j]!=hashzhi[i*W+(j+1)]) havejuchi=true;
                if (j!=0 && hashzhi[i*W+j]!=hashzhi[i*W+(j-1)]) havejuchi=true;
                if (i!=H-1 && hashzhi[i*W+j]!=hashzhi[(i+1)*W+j]) havejuchi=true;
                if (i!=0 && hashzhi[i*W+j]!=hashzhi[(i-1)*W+j]) havejuchi=true;
                if (havejuchi)
                {
                    //cout<<i<<' '<<j<<endl;
                    for (double ix=-0.25;ix<=0.25;ix=ix+0.25)
                        for (double iy=-0.25;iy<=0.25;iy=iy+0.25)
                        {
                            newi=i+ix/2; newj=j+iy/2;
                            nowhash=0;
                            csfx=nowscene.nowcamera->shechu(newi,newj);
                            csray.start=nowscene.nowcamera->wz;
                            csray.direct=csfx;
                            csray.direct.Normalize();
                            thiscolor=thiscolor+raytracing(csray,1,nowhash)/9;
                        }
                    thiscolor.Repaircolor();
                    nowscene.nowcamera->nowimage->drawpixel(i,j,thiscolor);
                }
            }
        }

        delete[]hashzhi;
    }
};

#endif // RAYTRACING_H

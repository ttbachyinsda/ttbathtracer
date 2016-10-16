#ifndef POLY_H
#define POLY_H
#include "object.h"
#include "triangle.h"
#include "ray_math_algo.h"
#include "kdtree.h"
class poly:public object
{
public:
    vector<triangle*> contain;
    vector<int> containnum1,containnum2,containnum3;
    vector<Vec3f> pointN;
    vector<Vec3f> premianN;
    kdtree nowtree;
    Vec3f wz;
    double bs;
    int reverse;
    void readfromfile(string filename)
    {
        bianhao=rand();
        next=0;
        ifstream input(filename);
        mt=new material();
        mt->readfromfile(input);
        input>>wz.x>>wz.y>>wz.z;
        input>>bs;
        input>>reverse;
        string mxname;
        input>>mxname;
        readfromobj(mxname);
    }
    void mdealer(vector<Vec3f> *points,double maxx,double maxy,double maxz,double minx,double miny,double minz)
    {
        double minn=maxx-minx;
        if (maxy-miny<minn) minn=maxy-miny;
        if (maxz-minz<minn) minn=maxz-minz;
        for (int i=0;i<points->size();i++)
        {
            points->at(i).x=(points->at(i).x-minx)/minn*bs;
            points->at(i).y=(points->at(i).y-miny)/minn*bs;
            points->at(i).z=(points->at(i).z-minz)/minn*bs;
            points->at(i)=points->at(i)+wz;
        }
    }
    static void moveup(vector<Vec3f> *points)
    {
        for (int i=0;i<points->size();i++)
        {
            swap(points->at(i).y,points->at(i).z);
        }
    }
    static void movearound(vector<Vec3f> *points)
    {
        for (int i=0;i<points->size();i++)
        {
            swap(points->at(i).x,points->at(i).y);
        }
    }
    static void reversex(vector<Vec3f> *points)
    {
        for (int i=0;i<points->size();i++)
            points->at(i).x=-points->at(i).x;
    }
    void readfromspeobj(string mxname)
    {
        string space;
        int vnum=0,fnum=0;
        ifstream input(objectpath(mxname));
        input>>space;
        string temp;
        vector<Vec3f> points,pointN;
        vector<int> cn1,cn2,cn3,cN1,cN2,cN3;
        double maxx=-1e8,minx=1e8,maxy=-1e8,miny=1e8,maxz=-1e8,minz=1e8;
        while (!input.eof())
        {
            if (space=="v")
            {
                vnum++;
                double x,y,z;
                input>>x>>y>>z;
                points.push_back(Vec3f(x,y,z));
                maxx=max(x,maxx);
                maxy=max(y,maxy);
                maxz=max(z,maxz);
                minx=min(x,minx);
                miny=min(y,miny);
                minz=min(z,minz);
            }
            if (space=="vn")
            {
                double x,y,z;
                input>>x>>y>>z;
                pointN.push_back(Vec3f(x,y,z));
            }
            if (space=="f")
            {
                fnum++;
                int num1,num2,num3;
                int numn1,numn2,numn3;
                char st;
                input>>num1>>st>>st>>numn1>>num2>>st>>st>>numn2>>num3>>st>>st>>numn3;
                cn1.push_back(num1);
                cn2.push_back(num2);
                cn3.push_back(num3);
                cN1.push_back(numn1);
                cN2.push_back(numn2);
                cN3.push_back(numn3);
            }
            getline(input,temp);
            input>>space;
        }
        if (mxname=="fixed.obj") moveup(&points);
        if (reverse) reversex(&points);
        mdealer(&points,maxx,maxy,maxz,minx,miny,minz);
        for (int i=0;i<vnum;i++)
        {
            maxx=max(points[i].x,maxx);
            maxy=max(points[i].y,maxy);
            maxz=max(points[i].z,maxz);
            minx=min(points[i].x,minx);
            miny=min(points[i].y,miny);
            minz=min(points[i].z,minz);
        }
        for (int i=0;i<fnum;i++)
        {
            triangle* nowtriangle=new triangle();
            nowtriangle->p0=points[cn1[i]-1];
            nowtriangle->p1=points[cn2[i]-1];
            nowtriangle->p2=points[cn3[i]-1];
            nowtriangle->bianhao=32768+rand();
            nowtriangle->next=0;
            nowtriangle->trianglenext=0;
            nowtriangle->mt=mt;
            nowtriangle->gash();
            nowtriangle->trueNp0=pointN[cN1[i]-1];
            nowtriangle->trueNp1=pointN[cN2[i]-1];
            nowtriangle->trueNp2=pointN[cn3[i]-1];
            contain.push_back(nowtriangle);
        }
        nowtree.root=new kdtreenode();
        nowtree.root->contain=contain;
        nowtree.root->box.point1=Vec3f(minx-EPS,miny-EPS,minz-EPS);
        nowtree.root->box.point2=Vec3f(maxx+EPS,maxy+EPS,maxz+EPS);
        nowtree.root->size=contain.size();
        nowtree.dividenode(nowtree.root,true,1);
    }

    void readfromobj(string mxname)
    {
        if (mxname=="dragon2.obj" || mxname=="fixed.obj")
        {
            readfromspeobj(mxname); return;
        }
        char space;
        int vnum=0,fnum=0;
        ifstream input(objectpath(mxname));
        input>>space;
        vector<Vec3f> points,premianN,pointN;
        double maxx=-1e8,minx=1e8,maxy=-1e8,miny=1e8,maxz=-1e8,minz=1e8;
        while (!input.eof())
        {
            if (space=='v')
            {
                vnum++;
                double x,y,z;
                input>>x>>y>>z;
                points.push_back(Vec3f(x,y,z));
                maxx=max(x,maxx);
                maxy=max(y,maxy);
                maxz=max(z,maxz);
                minx=min(x,minx);
                miny=min(y,miny);
                minz=min(z,minz);
            }
            if (space=='f')
            {
                fnum++;
                int num1,num2,num3;
                input>>num1>>num2>>num3;
                containnum1.push_back(num1-1);
                containnum2.push_back(num2-1);
                containnum3.push_back(num3-1);
            }
            input>>space;
        }
        if (mxname=="dragon.obj") moveup(&points);
        if (mxname=="bunny.obj") moveup(&points);
        if (mxname=="Buddha.obj") moveup(&points);
        if (mxname=="dinosaur.obj") movearound(&points);
        if (reverse) reversex(&points);
        mdealer(&points,maxx,maxy,maxz,minx,miny,minz);
        maxx=-1e8,minx=1e8,maxy=-1e8,miny=1e8,maxz=-1e8,minz=1e8;
        for (int i=0;i<vnum;i++)
        {
            maxx=max(points[i].x,maxx);
            maxy=max(points[i].y,maxy);
            maxz=max(points[i].z,maxz);
            minx=min(points[i].x,minx);
            miny=min(points[i].y,miny);
            minz=min(points[i].z,minz);
        }
        for (int i=0;i<fnum;i++)
        {
            int num1=containnum1[i];
            int num2=containnum2[i];
            int num3=containnum3[i];
            Vec3f p0=points[num1];
            Vec3f p1=points[num2];
            Vec3f p2=points[num3];
            Vec3f N=(p2-p0)*(p1-p0);
            premianN.push_back(N);
        }
        for (int i=0;i<vnum;i++)
            pointN.push_back(Vec3f(0,0,0));
        for (int i=0;i<fnum;i++)
        {
            pointN[containnum1[i]] += premianN[i];
            pointN[containnum2[i]] += premianN[i];
            pointN[containnum3[i]] += premianN[i];
        }
        for (int i=0;i<vnum;i++)
            pointN[i].Normalize();
        for (int i=0;i<fnum;i++)
        {
            triangle* nowtriangle=new triangle();
            nowtriangle->p0=points[containnum1[i]];
            nowtriangle->p1=points[containnum2[i]];
            nowtriangle->p2=points[containnum3[i]];

            nowtriangle->bianhao=32768+i+rand();
            nowtriangle->next=0;
            nowtriangle->trianglenext=0;
            nowtriangle->mt=mt;
            nowtriangle->gash();
            nowtriangle->trueNp0=pointN[containnum1[i]];
            nowtriangle->trueNp1=pointN[containnum2[i]];
            nowtriangle->trueNp2=pointN[containnum3[i]];
            contain.push_back(nowtriangle);
        }
        nowtree.root=new kdtreenode();
        nowtree.root->contain=contain;
        nowtree.root->box.point1=Vec3f(minx-EPS,miny-EPS,minz-EPS);
        nowtree.root->box.point2=Vec3f(maxx+EPS,maxy+EPS,maxz+EPS);
        nowtree.root->size=contain.size();
        nowtree.dividenode(nowtree.root,true,1);
    }
    bool pengzhuang(ray nowray, zhuang &info)
    {
        int newbianhao;
        bool yes=nowtree.getpengzhuang(nowtree.root,nowray,info,newbianhao);
        if (yes)
        {
            bianhao=newbianhao;
            return true;
        }
        return false;
    }
    color getcolor(zhuang info)
    {
        return color(1,1,1);
    }
    void outputdata()
    {
    }
};
#endif // POLY_H

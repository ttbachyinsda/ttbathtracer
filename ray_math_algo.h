#ifndef RAY_MATH_ALGO_H
#define RAY_MATH_ALGO_H
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include <fstream>
#include <map>
#include "Vec3f.h"
using namespace std;
#define EPS 1e-7
#define PI 3.1415926535897932385
#define rand01() (rand()%10001/10000.0)
double ray_sishewuru(double now)
{
    double a=floor(now),b=ceil(now);
    if (fabs(b-now)<=fabs(a-now)) return b;
     else return a;
}
string filepath(string x)
{
    string t="./";
    return t+x;
}
string objectpath(string x)
{
    string t="./scene/";
    return t+x;
}
string lightpath(string x)
{
    string t="./scene/";
    return t+x;
}
string scenepath(string x)
{
    string t="./scene/";
    return t+x;
}
string camerapath(string x)
{
    string t="./scene/";
    return t+x;
}
string materialpath(string x)
{
    string t="./scene/";
    return t+x;
}

double rand_at(double x,double y)
{
    double t=rand()%10001/10000.0;
    t *= (y-x);
    t += x;
    return t;
}
double sqr(double x)
{
    return x*x;
}
double min(double x,double y,double z)
{
    if (y<x) x=y;
    if (z<x) x=z;
    return x;
}
double min(double x,double y,double z,double q)
{
    if (y<x) x=y;
    if (z<x) x=z;
    if (q<x) x=q;
    return x;
}
double max(double x,double y,double z)
{
    if (y>x) x=y;
    if (z>x) x=z;
    return x;
}
double max(double x,double y,double z,double q)
{
    if (y>x) x=y;
    if (z>x) x=z;
    if (q>x) x=q;
    return x;
}
void safeprint(int i,int H)
{
    cout<<"now calculating: ("<<i<<") "<<H<<endl;
}
double det(Vec3f x1,Vec3f x2,Vec3f x3)
{
    double t1=x1.x*(x2.y*x3.z-x2.z*x3.y);
    double t2=-x1.y*(x2.x*x3.z-x2.z*x3.x);
    double t3=x1.z*(x2.x*x3.y-x3.x*x2.y);
    return t1+t2+t3;
}
double phong(Vec3f indirect,Vec3f outdirect,Vec3f N,double strength)
{
    Vec3f refl=indirect.reflect(N);
    double s=refl.dot(outdirect);
    if (s<EPS) return 0;
    return pow(s,strength);
}
double lambert(Vec3f indirect,Vec3f N)
{
    double s=indirect.dot(N);
    if (s<EPS) return 0;
    return s;
}

#endif // RAY_MATH_ALGO_H

#ifndef CAMERA_H
#define CAMERA_H
#include "myimg.h"
#include "ray.h"
#include "Vec3f.h"
#include <fstream>
#include "ray_math_algo.h"
class camera
{
public:
    int samples;
    Vec3f wz,N,vecx,vecy;
    double lenwidth,lenheight;
    int width,height;
    MyImg* nowimage;
    double shadecttime;
    double jingmianmanfancttime;
    bool focuson;
    double focustime,focuslen,aperture;
    Vec3f shechu(double i,double j)
    {
        return N+vecx*(2*i/height-1)+vecy*(2*j/width-1);
    }
    ray focusshechu(double i,double j)
    {
        Vec3f est=shechu(i,j); est.Normalize();
        Vec3f focuspoint=wz+(est*focuslen);
        double x,y;
        do{
            x=rand_at(0,1)*2-1;
            y=rand_at(0,1)*2-1;
        }while (x*x+y*y>1);
        ray outray;
        outray.start=wz+vecx*aperture*x+vecy*aperture*y;
        outray.direct=(focuspoint-outray.start).getnormalize();
        return outray;
    }

    void readfromfile(string filename)
    {
        ifstream input(filename);
        input>>samples;
        wz.reader(input);
        N.reader(input);
        input>>lenwidth>>lenheight>>width>>height;
        input>>shadecttime>>jingmianmanfancttime;
        input>>focuson;
        input>>focustime>>focuslen>>aperture;
        N.Normalize();
        vecx=N.getvertical();
        vecy=vecx*N;
        vecx *= lenheight/2;
        vecy *= lenwidth/2;
        nowimage= new MyImg();
        nowimage->height=height;
        nowimage->width=width;
        nowimage->painter(color(0,0,0),true);
    }
    void output(string filename)
    {
        nowimage->saveflg(filename);
    }
};

#endif // CAMERA_H

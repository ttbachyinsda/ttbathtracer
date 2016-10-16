#ifndef __MYIMG_H__
#define __MYIMG_H__
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include "ray_math_algo.h"
#include "color.h"
using namespace std;
class MyImg
{
public:
    stbi_uc * thisimage;
    int height;
    int width;
    MyImg(){}
    void LoadFromFile(string filename)
    {
        int comp,req_comp;
        thisimage=stbi_load(filename.c_str(),&width,&height,&comp,3);
        //_nowimage=cv::imread(filename,CV_LOAD_IMAGE_COLOR);
        //height=_nowimage.rows;
        //width=_nowimage.cols;
    }
    void saveflg(string filename)
    {
        stbi_write_bmp(filename.c_str(),width,height,3,thisimage);
        //cv::imwrite(filename,_nowimage);
    }

    void drawpixel(int x,int y,color nowcolor)
    {
        //uchar* data = _nowimage.data + x * _nowimage.step + y * _nowimage.elemSize();
        stbi_uc blue=(int)(ray_sishewuru(nowcolor.b*255)+EPS);
        stbi_uc green=(int)(ray_sishewuru(nowcolor.g*255)+EPS);
        stbi_uc red=(int)(ray_sishewuru(nowcolor.r*255)+EPS);
        thisimage[x*width*3+y*3]=red;
        thisimage[x*width*3+y*3+1]=green;
        thisimage[x*width*3+y*3+2]=blue;
    }
    color getcolor(int x,int y)
    {
        //uchar* data = _nowimage.data + x * _nowimage.step + y * _nowimage.elemSize();
        color nowcolor;
        nowcolor.r=thisimage[x*width*3+y*3]/255.0;
        nowcolor.g=thisimage[x*width*3+y*3+1]/255.0;
        nowcolor.b=thisimage[x*width*3+y*3+2]/255.0;
        return nowcolor;
    }
    color getsmoothcolor(double x,double y)
    {
        int minx=floor(x+EPS),miny=floor(y+EPS);
        int maxx=minx+1,maxy=miny+1;
        double xiaoshux=x-minx;
        double xiaoshuy=y-miny;
        color result(0,0,0);
        if (minx<0) cout<<minx<<endl;
        if (maxx>=height) maxx=0;
        if (maxy>=width) maxy=0;
        result=result+getcolor(minx,miny)*xiaoshux*xiaoshuy
                     +getcolor(minx,maxy)*xiaoshux*(1-xiaoshuy)
                     +getcolor(maxx,miny)*(1-xiaoshux)*xiaoshuy
                     +getcolor(maxx,maxy)*(1-xiaoshux)*(1-xiaoshuy);
        return result;
    }
    void painter(color newcolor,bool flag)
    {
        if (flag)
            thisimage= new stbi_uc[height*width*3];
        for (int i=0;i<height;i++)
            for (int j=0;j<width;j++)
                drawpixel(i,j,newcolor);
    }
    void changebrightness(double brightness)
    {
        color totalcolor(0,0,0);
        for (int i=0;i<height;i++)
            for (int j=0;j<width;j++)
                totalcolor += getcolor(i,j);
        totalcolor /= height*width;
        for (int i=0;i<height;i++)
            for (int j=0;j<width;j++)
            {
                color nowcolor=getcolor(i,j);
                nowcolor += totalcolor*brightness;
                drawpixel(i,j,nowcolor);
            }
    }
    void rotate()
    {
        int preheight=height,prewidth=width;

        vector<color> se;

        for (int i=0;i<preheight;i++)
            for (int j=0;j<prewidth;j++)
                se.push_back(getcolor(i,j));
        int p=height; height=width; width=p;

        painter(color(0,0,0),false);
        for (int i=0;i<height;i++)
            for (int j=0;j<width;j++)
            {
                drawpixel(i,j,se[j*prewidth+height-i-1]);
                //cout<<i<<' '<<j<<' '<<j<<' '<<height-i-1<<' '<<j*prewidth+height-i-1<<' '<<preheight<<' '<<prewidth<<endl;
            }
    }
};

#endif // __MYIMG_H__

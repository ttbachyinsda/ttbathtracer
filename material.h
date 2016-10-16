#ifndef MATERIAL_H
#define MATERIAL_H
#include "myimg.h"
#include "color.h"
#include <fstream>
#include "ray_math_algo.h"
class material
{
public:
    MyImg* background;
    double pfan,pzhe,pmanfan,pjingmianmanfan,rjingmianmanfan;
    double zheshelv;
    color mcolor,acolor;
    material()
    {
        background=0;
        pfan=0; pzhe=0; pmanfan=0; pjingmianmanfan=0; rjingmianmanfan=0;
        zheshelv=0;
    }
    void readfromfile(ifstream &input)
    {
        input>>pfan>>pzhe>>pmanfan>>pjingmianmanfan>>rjingmianmanfan;
        input>>zheshelv;
        mcolor.reader(input);
        acolor.reader(input);
        string picname;
        input>>picname;
        if (picname=="no")
        {
            background=0;
            return;
        }
        background = new MyImg();
        background->LoadFromFile(materialpath(picname));
    }
    int dubo()
    {
        double ss=rand01();
        double total=pfan+pzhe+pmanfan+pjingmianmanfan;
        if (ss*total<=pfan) return 1;
        if (ss*total<=pfan+pzhe) return 2;
        if (ss*total<=pfan+pzhe+pmanfan) return 3;
        return 4;
    }
    color getcolor(double x,double y)
    {
        return background->getsmoothcolor(x,y);
    }
};

#endif // MATERIAL_H

#ifndef SCENE_H
#define SCENE_H
#include "ball.h"
#include "triangle.h"
#include "poly.h"
#include "pointlight.h"
#include "plain.h"
#include "squarelight.h"
#include "ray_math_algo.h"
#include "camera.h"
#include "light.h"
#include "bdbox.h"
class scene
{
public:
    object* nowobj;
    light* nowlight;
    color background;
    camera* nowcamera;
    ~scene()
    {
        delete nowcamera;
        object* s=nowobj;
        while (s)
        {
            object *p=s;
            s=s->next;
            delete p;
        }
        light * t=nowlight;
        while (t)
        {
            light *r=t;
            t=t->next;
            delete r;
        }
    }

    void readfromfile(string filename)
    {
        ifstream input(filename);
        background=color(0.6,0.6,0.6);
        input>>background.r>>background.g>>background.b;
        //background=color(0,0,0);
        nowobj=0;
        nowlight=0;
        nowcamera=0;
        object* tailobject=0;
        light* taillight=0;
        while (true)
        {
            string x,y;
            input>>x;
            if (x=="end") break;
            input>>y;
            if (x=="ball")
            {
                object* newobj = new ball();
                newobj->readfromfile(objectpath(y));
                if (nowobj)
                {
                    tailobject->next=newobj;
                    tailobject=newobj;
                }
                else nowobj=newobj,tailobject=newobj;
            }
            if (x=="bdbox")
            {
                object* newobj = new bdbox();
                newobj->readfromfile(objectpath(y));
                if (nowobj)
                {
                    tailobject->next=newobj;
                    tailobject=newobj;
                }
                else nowobj=newobj,tailobject=newobj;
            }
            if (x=="plain")
            {
                object* newobj = new plain();
                newobj->readfromfile(objectpath(y));
                if (nowobj)
                {
                    tailobject->next=newobj;
                    tailobject=newobj;
                }
                else nowobj=newobj,tailobject=newobj;
            }
            if (x=="triangle")
            {
                object* newobj = new triangle();
                newobj->readfromfile(objectpath(y));
                if (nowobj)
                {
                    tailobject->next=newobj;
                    tailobject=newobj;
                }
                else nowobj=newobj,tailobject=newobj;
            }
            if (x=="poly")
            {
                object* newobj = new poly();
                newobj->readfromfile(objectpath(y));
                if (nowobj)
                {
                    tailobject->next=newobj;
                    tailobject=newobj;
                }
                else nowobj=newobj,tailobject=newobj;
            }
            if (x=="pointlight")
            {
                light* newlight = new pointlight();
                newlight->readfromfile(lightpath(y));
                if (nowlight)
                {
                    taillight->next=newlight;
                    taillight=newlight;
                }
                else nowlight=newlight,taillight=newlight;
            }
            if (x=="squarelight")
            {
                light* newlight = new squarelight();
                newlight->readfromfile(lightpath(y));
                if (nowlight)
                {
                    taillight->next=newlight;
                    taillight=newlight;
                }
                else nowlight=newlight,taillight=newlight;
            }
            if (x=="camera")
            {
                nowcamera=new camera();
                nowcamera->readfromfile(camerapath(y));
            }
        }
    }
    object* findnearobj(ray nowray,zhuang &info)
    {
        zhuang minn;
        object* res=0;
        minn.dist=1e+35;
        zhuang temp;
        for (object*thi=nowobj;thi;thi=thi->next)
        {
            if (thi->pengzhuang(nowray,temp))
            {
                if (res==0 || temp.dist<minn.dist)
                {
                    minn=temp;
                    res=thi;
                }
            }
        }
        info=minn;
        return res;
    }
    light* findnearlight(ray nowray,double &info)
    {
        double minn;
        light* res=0;
        minn=1e+35;
        double temp;
        for (light*thi=nowlight;thi;thi=thi->next)
        {
            if (thi->pengzhuang(nowray,temp))
                if (res==0 || temp<minn)
                {
                    minn=temp;
                    res=thi;
                }
        }
        info=minn;
        return res;
    }
};

#endif // SCENE_H

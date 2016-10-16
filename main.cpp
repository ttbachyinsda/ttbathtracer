#include "myimg.h"
#include "ray_math_algo.h"
#include "Vec3f.h"
#include "ball.h"
#include "plain.h"
#include "camera.h"
#include "scene.h"
#include "raytracing.h"
#include "pathtracing.h"
int main(int argc, char *argv[])
{

    if (argc!=4)
    {
        cout<<"Please use command ./raytracing -uninf scene6.txt result2.bmp to produce an test image"<<endl;
        return 0;
    }
    bool infinite;
    string s1(argv[1]),s2(argv[2]),s3(argv[3]);
    if (s1=="-inf") infinite=true; else infinite=false;
    pathtracer small;
    small.nowscene.readfromfile(scenepath(s2));
    small.run(small.nowscene.nowcamera->samples,infinite);
    small.nowscene.nowcamera->nowimage->rotate();
    small.nowscene.nowcamera->output(filepath(s3));
    return 0;
}

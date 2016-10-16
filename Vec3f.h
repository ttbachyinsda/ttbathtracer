#ifndef vec3f_h
#define vec3f_h
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#define EPS 1e-7
#define PI 3.1415926535897932385
#define rand01() (rand()%10001/10000.0)
using namespace std;
    class Vec3f
    {
    public:

        //Constructors
        Vec3f();
        Vec3f(double x,double y, double z);
        //Vec3f(const Vec3f& v);
        //Deconstructor
    public:
        //Operators

        //Operator []
        double& operator [](int index)
        {
            return _p[index];
        }
        const double& operator [](int index) const
        {
            return _p[index];
        }
        //Operator =
        Vec3f& operator = (const Vec3f& v);

        //Operators +=,-=, *=, /=
        void operator +=(const Vec3f& v);
        void operator +=(double f);
        void operator -=(const Vec3f& v);
        void operator -=(double f);
        void operator *=(const Vec3f& v);
        void operator *=(double f);
        void operator /=(const Vec3f& v);
        void operator /=(double f);

        //Operators +,-.*,/
        Vec3f operator +(const Vec3f&v) const;
        Vec3f operator +(double f) const;
        Vec3f operator -(const Vec3f&v) const;
        Vec3f operator -(double f) const;
        Vec3f operator *(const Vec3f&v) const;
        Vec3f operator *(double f) const;
        Vec3f operator /(const Vec3f&v) const;
        Vec3f operator /(double f) const;

        Vec3f operator -() const;

    public:
        void Normalize();
        double sqr();
        void print();
        double mole();
        double dot(Vec3f b2);
        Vec3f realcheng(Vec3f b2);
        Vec3f reflect(Vec3f mian);
        Vec3f zheshe(Vec3f mian,double n);
        Vec3f getnormalize();
        bool iszero();
        Vec3f getvertical();
        bool canzheshe(Vec3f mian,double n,Vec3f &out);


    public:
        union
        {
            struct
            { double _p[3]; };
            struct
            { double x,y,z; };
            struct
            { double r,g,b; };
        };
        void reader(ifstream &input)
        {
            input>>x>>y>>z;
        }
        enum {_len = 3};   
        
    };

#endif

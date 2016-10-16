#include "Vec3f.h"
#include <math.h>
#include <cstdlib>

//////////////////////////////////////////////////////////////////////////
//  Constructors and Deconstructors
    Vec3f::Vec3f(void)
    {
        memset(_p,0,sizeof(double)*_len);
    }
    
    Vec3f::Vec3f(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
/*
    Vec3f::Vec3f(const Vec3f &v)
    {
        memcpy(_p,v._p,sizeof(double)*_len);
    }
*/

//////////////////////////////////////////////////////////////////////////
// Operators

    Vec3f& Vec3f::operator =( const Vec3f& v)
    {
        memcpy(_p,v._p,sizeof(double)*_len);
        return (*this);
    }

    void Vec3f::operator +=(const Vec3f& v)
    {
        for(int i=0;i<_len;i++)
            _p[i] += v._p[i];
    }
    void Vec3f::operator +=(double f)
    {
        for(int i=0;i<_len;i++)
            _p[i] += f;
    }

    void Vec3f::operator -=(const Vec3f& v)
    {
        for(int i=0;i<_len;i++)
            _p[i] -= v._p[i];
    }
    void Vec3f::operator -=(double f)
    {
        for(int i=0;i<_len;i++)
            _p[i] -= f;
    }

    void Vec3f::operator *=(const Vec3f& v)
    {
        *this = Vec3f(_p[1]*v._p[2]-_p[2]*v._p[1],_p[2]*v._p[0]-_p[0]*v._p[2],_p[0]*v._p[1]-_p[1]*v._p[0]);
    }
    void Vec3f::operator *=(double f)
    {
        for(int i=0;i<_len;i++)
            _p[i] *= f;
    }

    void Vec3f::operator /=(const Vec3f& v)
    {
        for(int i=0;i<_len;i++)
            _p[i] /= v._p[i];
    }
    void Vec3f::operator /=(double f)
    {
        for(int i=0;i<_len;i++)
            _p[i] /= f;
    }

    Vec3f Vec3f::operator +(const Vec3f&v) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] + v[i];
        return res;
    }
    Vec3f Vec3f::operator +(double f) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] + f;
        return res;
    }

    Vec3f Vec3f::operator -(const Vec3f&v) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] - v[i];
        return res;
    }
    Vec3f Vec3f::operator -(double f) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] - f;
        return res;
    }

    Vec3f Vec3f::operator *(const Vec3f&v) const
    {
        return Vec3f(_p[1]*v._p[2]-_p[2]*v._p[1],_p[2]*v._p[0]-_p[0]*v._p[2],_p[0]*v._p[1]-_p[1]*v._p[0]);
    }
    Vec3f Vec3f::operator *(double f) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] * f;
        return res;
    }

    Vec3f Vec3f::operator /(const Vec3f&v) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] / v[i];
        return res;
    }
    Vec3f Vec3f::operator /(double f) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] / f;
        return res;
    }

    Vec3f Vec3f::operator - () const 
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = -(*this)[i];
        return res;
    }

//////////////////////////////////////////////////////////////////////////
// Other Methods
    void Vec3f::Normalize()
    {
        double c=x;
        double fSqr = sqr();
        if(fSqr>1e-6)
            (*this) *= 1.0f/sqrt(fSqr);
        double d=x;
        if (c*d<0) cout<<"errorr"<<endl;
    }
    double Vec3f::sqr()
    {
        return _p[0]*_p[0] + _p[1]*_p[1] + _p[2]*_p[2];
    }
    void Vec3f::print()
    {
        cout<<"x:"<<x<<' ';
        cout<<"y:"<<y<<' ';
        cout<<"z:"<<z<<' ';
    }
    double Vec3f::mole()
    {
        return sqrt(_p[0]*_p[0] + _p[1]*_p[1] + _p[2]*_p[2]);
    }
    Vec3f Vec3f::getnormalize()
    {
        return *this / mole();
    }
    Vec3f Vec3f::realcheng(Vec3f b2)
    {
        return Vec3f(_p[0]*b2._p[0],_p[1]*b2._p[1],_p[2]*b2._p[2]);
    }

    double Vec3f::dot(Vec3f b2)
    {
        return _p[0]*b2._p[0]+_p[1]*b2._p[1]+_p[2]*b2._p[2];
    }
    Vec3f Vec3f::reflect(Vec3f mian)
    {
        mian.Normalize();
        return *this - mian*(2*dot(mian));
    }
    bool Vec3f::canzheshe(Vec3f mian, double n,Vec3f &out)
    {
        Vec3f danwei = getnormalize();
        double cosjiao1 = -mian.dot(danwei);
        double cosjiao2fang = 1-(n*n)*(1-cosjiao1*cosjiao1);
        if (cosjiao2fang>EPS)
        {
            out=danwei*n + mian*(n*cosjiao1-sqrt(cosjiao2fang));
            return true;
        }
        else
        {
            out=danwei.reflect(mian);
            return false;
        }
    }
    Vec3f Vec3f::zheshe(Vec3f mian, double n)
    {
        Vec3f danwei = getnormalize();
        double cosjiao1 = -mian.dot(danwei);
        double cosjiao2fang = 1-(n*n)*(1-cosjiao1*cosjiao1);
        if (cosjiao2fang>EPS) return danwei*n + mian*(n*cosjiao1-sqrt(cosjiao2fang));
           else return danwei.reflect(mian);
    }
    bool Vec3f::iszero()
    {
        if (fabs(_p[0])<EPS && fabs(_p[1])<EPS && fabs(_p[2])<EPS) return true;
          else return false;
    }

    Vec3f Vec3f::getvertical() {
        Vec3f ret = *this * Vec3f( 0 , 0 , 1 );
        if ( ret.iszero() ) ret = Vec3f( 1 , 0 , 0 );
            else ret = ret.getnormalize();
        return ret;
    }

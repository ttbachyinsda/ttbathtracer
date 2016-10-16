#ifndef color_H
#define color_H
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <fstream>
using namespace std;

class color
{
public:
    double r,g,b;
    color(double nr=0,double ng=0,double nb=0):r(nr),g(ng),b(nb){}
    ~color(){}
    friend color operator + ( const color , const color );
    friend color operator - ( const color , const color );
    friend color operator * ( const color , const color );
    friend color operator * ( const color , const double );
    friend color operator / ( const color , const double );
    friend color operator += ( color , const color );
    friend color operator -= ( color , const color );
    friend color operator *= ( color , const double );
    friend color operator /= ( color , const double );
    void Repaircolor();
    void print();
    void reader(ifstream &input)
    {
        input>>r>>g>>b;
    }
};
color operator + ( const color nowa , const color nowb ) {
    return color( nowa.r + nowb.r , nowa.g + nowb.g , nowa.b + nowb.b );
}
color operator - ( const color nowa , const color nowb ) {
    return color( nowa.r - nowb.r , nowa.g - nowb.g , nowa.b - nowb.b );
}
color operator * ( const color nowa , const color nowb ) {
    return color( nowa.r * nowb.r , nowa.g * nowb.g , nowa.b * nowb.b );
}
color operator * ( const color nowa , const double k ) {
    return color( nowa.r * k , nowa.g * k , nowa.b * k );
}
color operator / ( const color nowa , const double k ) {
    return color( nowa.r / k , nowa.g / k , nowa.b / k );
}
color operator += ( color nowa , const color nowb ) {
    nowa = nowa + nowb;
    return nowa;
}
color operator -= ( color nowa , const color nowb ) {
    nowa = nowa - nowb;
    return nowa;
}
color operator *= ( color nowa , const double k ) {
    nowa = nowa * k;
    return nowa;
}

color operator /= ( color nowa , const double k ) {
    nowa = nowa / k;
    return nowa;
}
void color::Repaircolor() {
    if ( r > 1 ) r = 1;
    if ( g > 1 ) g = 1;
    if ( b > 1 ) b = 1;
}
void color::print()
{
    cout<<"r:"<<r<<' ';
    cout<<"g:"<<g<<' ';
    cout<<"b:"<<b<<endl;
}
#endif // color_H

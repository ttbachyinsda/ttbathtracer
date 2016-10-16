#ifndef KDTREENODE_H
#define KDTREENODE_H
#include "bdbox.h"
#include "triangle.h"
#include "ray_math_algo.h"
class kdtreenode
{
public:
    vector<triangle*> contain;
    int size; int fenlei;
    double splitwz;
    kdtreenode* left;
    kdtreenode* right;
    bdbox box;
    kdtreenode()
    {
        size=0; fenlei=-1; splitwz=0; left=0; right=0;
    }
};

#endif // KDTREENODE_H

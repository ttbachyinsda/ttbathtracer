#ifndef KDTREE_H
#define KDTREE_H
#include "kdtreenode.h"
class kdtree
{
public:
    kdtreenode *root;
    bool xiaoyu(triangle* x,triangle* y,int wz, bool ismin)
    {
        if (ismin)
            return x->mins[wz]<y->mins[wz];
        else
            return x->maxs[wz]<y->maxs[wz];
    }
    void sorttriangle(vector<triangle*> &contain,int l,int r,int wz,bool ismin)
    {
        int i=l,j=r;
        triangle* mid=contain[(l+r)/2];
        do{
            while (xiaoyu(contain[i],mid,wz,ismin)) i++;
            while (xiaoyu(mid,contain[j],wz,ismin)) j--;
            if (i<=j)
            {
                triangle*p=contain[i]; contain[i]=contain[j]; contain[j]=p;
                i++; j--;
            }
        }while (i<=j);
        if (i<r) sorttriangle(contain,i,r,wz,ismin);
        if (j>l) sorttriangle(contain,l,j,wz,ismin);
    }
    void dividenode(kdtreenode *now,bool canfen,int depth)
    {
bool flag=false;
        /*
        cout<<'r'<<endl;
        now->box.point1.print();
        now->box.point2.print();
        now->box.pdin(Vec3f(0,0,0));
        cout<<'e'<<endl;
        for (int i=0;i<now->size;i++)
        {
            int sj=0;
            triangle* s=now->contain[i];
            if (s->p0.x<now->box.point1.x) sj=1;
            if (s->p1.x<now->box.point1.x) sj=1;
            if (s->p2.x<now->box.point1.x) sj=1;
            if (s->p0.x>now->box.point2.x) sj=1;
            if (s->p1.x>now->box.point2.x) sj=1;
            if (s->p2.x>now->box.point2.x) sj=1;

            if (s->p0.y<now->box.point1.y) sj=1;
            if (s->p1.y<now->box.point1.y) sj=1;
            if (s->p2.y<now->box.point1.y) sj=1;
            if (s->p0.y>now->box.point2.y) sj=1;
            if (s->p1.y>now->box.point2.y) sj=1;
            if (s->p2.y>now->box.point2.y) sj=1;

            if (s->p0.z<now->box.point1.z) sj=1;
            if (s->p1.z<now->box.point1.z) sj=1;
            if (s->p2.z<now->box.point1.z) sj=1;
            if (s->p0.z>now->box.point2.z) sj=1;
            if (s->p1.z>now->box.point2.z) sj=1;
            if (s->p2.z>now->box.point2.z) sj=1;
            if (sj==1)
            {
                cout<<"error"<<endl;
            }

        }
        */
        vector<triangle*> minnode;
        vector<triangle*> maxnode;
        for (int i=0;i<now->size;i++)
        {
            minnode.push_back(now->contain[i]);
            maxnode.push_back(now->contain[i]);
        }
        double mincost=1e+35,bestsplit=0;
        int mint=-1; int leftsize=0,rightsize=0,bestwz=0;
        for (int i=0;i<3;i++)
        {
            //if (fabs(now->box.point1._p[i]-now->box.point2._p[i])<0.025) continue;
            sorttriangle(minnode,0,now->size-1,i,true);
            sorttriangle(maxnode,0,now->size-1,i,false);
            bdbox leftbox=now->box,rightbox=now->box;
            Vec3f prepoint2=leftbox.point2,prepoint1=rightbox.point1;
            int nowwz=0;
            for (int j=0;j<now->size;j++)
            {
                leftbox.point2=prepoint2;
                rightbox.point1=prepoint1;
                double nowsplit=minnode[j]->mins[i];
                //if (nowsplit<leftbox.point1._p[i]+0.025) continue;
                //if (nowsplit<rightbox.point2._p[i]-0.025) continue;
                leftbox.point2._p[i]=nowsplit;
                rightbox.point1._p[i]=nowsplit;
                while (nowwz<now->size && maxnode[nowwz]->maxs[i]<=nowsplit+EPS) nowwz++;
                double nowcost=leftbox.getmianji()*j+rightbox.getmianji()*(now->size-nowwz);
                if (nowcost<mincost)
                {
                    mincost=nowcost;
                    leftsize=j; rightsize=now->size-nowwz; bestwz=nowwz; bestsplit=nowsplit; mint=i;
                }
            }

            nowwz=0;
            for (int j=0;j<now->size;j++)
            {
                leftbox.point2=prepoint2;
                rightbox.point1=prepoint1;
                double nowsplit=maxnode[j]->maxs[i];
                //if (nowsplit<leftbox.point1._p[i]+0.025) continue;
                //if (nowsplit<rightbox.point2._p[i]-0.025) continue;
                leftbox.point2._p[i]=nowsplit;
                rightbox.point1._p[i]=nowsplit;
                while (nowwz<now->size && minnode[nowwz]->mins[i]<=nowsplit+EPS) nowwz++;
                double nowcost=leftbox.getmianji()*nowwz+rightbox.getmianji()*(now->size-j);
                if (nowcost<mincost)
                {
                    mincost=nowcost;
                    leftsize=nowwz; rightsize=now->size-j; bestwz=nowwz; bestsplit=nowsplit; mint=i;
                }
            }
        }
        if (mint!=-1)
        {
            int realleftsize=0,realrightsize=0;
            bdbox leftbox=now->box,rightbox=now->box;
            leftbox.point2[mint]=bestsplit;
            rightbox.point1[mint]=bestsplit;
            for (int i=0;i<now->size;i++)
            {
                if (now->contain[i]->mins[mint]<=bestsplit+EPS || now->contain[i]->maxs[mint]<=bestsplit+EPS)
                {
                    realleftsize++;
                    if (now->contain[i]->mins[mint]<leftbox.point1[mint])
                    {
                        leftbox.point1[mint]=now->contain[i]->mins[mint];
                    }
                    if (now->contain[i]->maxs[mint]>leftbox.point2[mint])
                    {
                        leftbox.point2[mint]=now->contain[i]->maxs[mint];
                    }
                }
                if (now->contain[i]->maxs[mint]>=bestsplit-EPS || now->contain[i]->mins[mint]>=bestsplit-EPS)
                {
                    realrightsize++;
                    if (now->contain[i]->mins[mint]<rightbox.point1[mint])
                    {
                        rightbox.point1[mint]=now->contain[i]->mins[mint];
                    }
                    if (now->contain[i]->maxs[mint]>rightbox.point2[mint])
                    {
                        rightbox.point2[mint]=now->contain[i]->maxs[mint];
                    }
                }
            }
            double cost=leftbox.getmianji()*realleftsize+rightbox.getmianji()*realrightsize;
            double maxcost=now->box.getmianji()*(now->size-1);
            if (cost<maxcost && canfen && realleftsize && realrightsize) //fou ze jiu bu yong fen le hao ba
            {
                //cout<<leftbox.point1._p[mint]<<' '<<bestsplit<<' '<<rightbox.point2._p[mint]<<endl;
                if (realleftsize+realrightsize<now->size)
                {
                    cout<<"error"<<endl;
                }
                now->fenlei=mint;
                now->splitwz=bestsplit;
                now->left=new kdtreenode();
                now->left->box=leftbox;

                now->right=new kdtreenode();
                now->right->box=rightbox;

                realleftsize=0; realrightsize=0;
                for (int k=0;k<now->size;k++)
                {
                    int has=0;
                    if (now->contain[k]->maxs[mint]<=leftbox.point2[mint])
                    {
                        realleftsize++;
                        now->left->contain.push_back(now->contain[k]),has=1;

                    }
                    if (now->contain[k]->mins[mint]>=rightbox.point1[mint])
                    {
                        realrightsize++;
                        now->right->contain.push_back(now->contain[k]),has=2;
                    }
                    if (!has) cout<<"error"<<endl;
                }
                cost=leftbox.getmianji()*realleftsize+rightbox.getmianji()*realrightsize;
                if (cost>=maxcost)
                {
                    delete now->left;
                    delete now->right;
                    now->left=0;
                    now->right=0;
                }
                else
                {
                now->left->size=realleftsize;
                now->right->size=realrightsize;
                flag=true;
                dividenode(now->left,canfen,depth+1);
                dividenode(now->right,canfen,depth+1);
                }
            }
        }
    }
    bool getpengzhuang(kdtreenode* now,ray nowray,zhuang& info,int &getbianhao)
    {
        //if (now==root) cout<<"this is"<<endl;
        //cout<<(&now)<<' '<<(&now->left)<<' '<<(&now->right)<<endl;
        nowray.direct.Normalize();
        if (!now->box.pdin(nowray.start) && now->box.quickpengzhuang(nowray)==-1) return false;
        if (!now->left && !now->right)
        {
            zhuang mininfo; int mint=-1;
            for (int i=0;i<now->size;i++)
            {
                zhuang tempinfo;
                bool yes=now->contain[i]->pengzhuang(nowray,tempinfo);
                if (yes)
                {
                    if (mint==-1 || tempinfo.dist<mininfo.dist)
                    {
                        mint=i;
                        mininfo=tempinfo;
                    }
                }
            }
            if (mint==-1) return false;
            else
            {
                getbianhao=now->contain[mint]->bianhao;
                info=mininfo;
                return true;
            }
        }
        if (now->left && now->left->box.pdin(nowray.start))  //first left then right
        {
            zhuang tempinfo;
            bool yes=getpengzhuang(now->left,nowray,tempinfo,getbianhao);
            if (yes)
            {
                info=tempinfo;
                return true;
            }
            yes=getpengzhuang(now->right,nowray,tempinfo,getbianhao);
            if (yes)
            {
                info=tempinfo;
                return true;
            }
            return false;
        }
        if (now->right && now->right->box.pdin(nowray.start))  //first right then left
        {
            zhuang tempinfo;
            bool yes=getpengzhuang(now->right,nowray,tempinfo,getbianhao);
            if (yes)
            {
                info=tempinfo;
                return true;
            }
            yes=getpengzhuang(now->left,nowray,tempinfo,getbianhao);
            if (yes)
            {
                info=tempinfo;
                return true;
            }
            return false;
        }
        double leftdist=-1,rightdist=-1;
        bool isright=false;
        if (now->left)
        leftdist=now->left->box.quickpengzhuang(nowray);
        if (now->right)
        rightdist=now->right->box.quickpengzhuang(nowray);
        if (leftdist==-1 && rightdist==-1) return false;
        if (leftdist==-1) isright=true;
        if (leftdist>rightdist && rightdist!=-1) isright=true;
        if (isright)
        {
            zhuang tempinfo;
            bool yes=getpengzhuang(now->right,nowray,tempinfo,getbianhao);
            if (yes)
            {
                info=tempinfo;
                return true;
            }
            if (leftdist!=-1)
            {
                yes=getpengzhuang(now->left,nowray,tempinfo,getbianhao);
                if (yes)
                {
                    info=tempinfo;
                    return true;
                }
            }
            return false;
        }
        else
        {
            zhuang tempinfo;
            bool yes=getpengzhuang(now->left,nowray,tempinfo,getbianhao);
            if (yes)
            {
                info=tempinfo;
                return true;
            }
            if (rightdist!=-1)
            {
                yes=getpengzhuang(now->right,nowray,tempinfo,getbianhao);
                if (yes)
                {
                    info=tempinfo;
                    return true;
                }
            }
            return false;
        }
        return false;
    }
    void getallsize(kdtreenode* now,int depth)
    {
    }
};

#endif // KDTREE_H

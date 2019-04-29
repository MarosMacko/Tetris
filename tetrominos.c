//
// Created by marosm on 3/15/19.
//
#include "tetrominos.h"

bool blockPosition(struct t_stats *g, short y, short x)
{
    g->blockRotation+=4; //Normalize to <0; 3>
    g->blockRotation%=4;

    switch (blockShape)
    {
        case shape_O:
        {
            return ((y<2) && (x%3>0));
        }

        case shape_I:
        {
            if (g->blockRotation%2) return (y==0);
            else return (x==1);
        }

        case shape_L:
        {
            switch(g->blockRotation)
            {
                case 0:
                    return (x==1 && y<3) || (x==2 && y==2);

                case 1:
                    return (y==1 && x>0) || (x==1 && y==2);

                case 2:
                    return (x==2 && y<3) || (y==0 && x==1);

                case 3:
                    return (y==2 && x<3) || (x==2 && y==1);

                default:
                    return false;
            }
        }

        case shape_J:
        {
            switch(g->blockRotation)
            {
                case 0:
                    return (x==2 && y<3) || (x==1 && y==2);

                case 1:
                    return (y==2 && x>0) || (x==1 && y==1);

                case 2:
                    return (x==1 && y<3) || (x==2 && y==0);

                case 3:
                    return (y==1 && x<3) || (x==2 && y==2);

                default:
                    return false;
            }
        }

        case shape_S:
        {
            if(g->blockRotation%2)
                return (x==1 && y<3 && y>0) || (x==2 && y<2);
            else
                return (y==1 && x>0 && x<3) || (y==2 && x>1);
        }

        case shape_Z:
        {
            if(g->blockRotation%2)
                return (x==2 && y<3 && y>0) || (x==1 && y<2);
            else
                return (y==2 && x>0 && x<3) || (y==1 && x>1);
        }

        case shape_T:
        {
            if(y==1 && x==1) return true;
            switch (g->blockRotation)
            {
                case 0://up right down
                    return (x==1 && (y==0 || y==2)) || (y==1 && x==2);

                case 1://right down left
                    return (y==1 && (x==0 || x==2)) || (y==2 && x==1);

                case 2://down left up
                    return (x==1 && (y==0 || y==2)) || (y==1 && x==0);

                case 3://left up right
                    return (y==1 && (x==0 || x==2)) || (y==0 && x==1);
                default: return false;
            }

        }
        default: return false;
    }

}

void shuffleSequence(struct t_stats *g)
{
    unsigned long result=0;
    int tetrominos[SHAPES];
    for(int i=0; i<SHAPES; i++) tetrominos[i]=i;

    unsigned nToGenerate = SHAPES;
    while(nToGenerate>0)
    {
        unsigned i = pcg32_boundedrand(SHAPES);
        if(tetrominos[i] != -1)
        {
            result*=10;
            result+=tetrominos[i];
            tetrominos[i]=-1;
            nToGenerate--;
        }
    }
    g->blockSequence=result;
}

int advanceSequence(struct t_stats *g)
{
    int result;
    if(g->blockSequence==0) shuffleSequence(g);

    result = g->blockSequence%10;
    g->blockSequence/=10;

    return result;
}

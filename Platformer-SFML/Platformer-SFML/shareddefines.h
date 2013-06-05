#ifndef __SHAREDDEFINES_H
#define __SHAREDDEFINES_H

enum TypeId
{
    TYPEID_ENEMY    = 0,
    TYPEID_PLAYER   = 1,
};

inline int urand(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}

inline bool IsInRange(float currX, float xDest, float currY, float yDest, float distance)
{
    return ((currX < xDest - distance && currX > xDest + distance && currY < yDest - distance && currY > yDest + distance) ||
            (currX > xDest - distance && currX < xDest + distance && currY > yDest - distance && currY < yDest + distance));
}

//void Load
#endif

#include <cmath>
#include <stdlib.h>

inline int urand(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}

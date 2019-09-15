#include "AreaMap.h"

AreaMap::AreaMap()
 : MappingData((char*)"/home/pi/AreaMap.dat")
{
    /* nop. */
}

AreaMap::~AreaMap()
{
    /* nop. */
}

/* 指定した座標が移動可能か判断する */
bool AreaMap::IsMovable(const unsigned long x, const unsigned long y)
{
    bool retVal = false;
    unsigned long value = 0;

    if (isInRange(x, y) != true)
    {
        goto FINISH;
    }

    value = Get(x, y);
    if ((value != MOVABLE_VALUE) && (value != OBSTACLE_VALUE))
    {
        goto FINISH;
    }

    retVal = true;

FINISH:
    return retVal;
}

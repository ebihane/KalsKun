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

/* �w�肵�����W���ړ��\�����f���� */
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

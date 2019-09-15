#pragma once

#include "MappingData.h"

class AreaMap : public MappingData
{
public :

    AreaMap();
    virtual ~AreaMap();

    /* 指定した座標が移動可能か判断する */
    bool IsMovable(const unsigned long x, const unsigned long y);

protected :


private :

    static const unsigned long  UNKNOWN_VALUE = 0;      /* 未確定 */
    static const unsigned long  MOVABLE_VALUE = 1;      /* 移動可能 */
    static const unsigned long  NOT_MOVABLE_VALUE = 2;  /* 移動不可 */
    static const unsigned long  OBSTACLE_VALUE = 3;     /* 障害物 */

};
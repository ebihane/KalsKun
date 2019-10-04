#pragma once

#include "Parts/ShareMemory/ShareMemory.h"

class TudeConverter
{
public :
    
    TudeConverter() {};
    virtual ~TudeConverter() {};

    /* 二つの緯経間の距離を換算する */
    static double ToDistance(TudeStr* const base, TudeStr* const target);

protected :

private :

    /* 地球半径 */
    static const double EARTH_R = 6378.137;

    /* 角度 → ラジアン 変換 */
    static double degreeToRadian(const double degree);

    /* ラジアン → 角度 変換 */
    static double radianToDegree(const double radian);
};

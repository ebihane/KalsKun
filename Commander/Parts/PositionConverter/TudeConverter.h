#pragma once

#include "Parts/ShareMemory/ShareMemory.h"

class TudeConverter
{
public :
    
    TudeConverter() {};
    virtual ~TudeConverter() {};

    /* ��̈܌o�Ԃ̋��������Z���� */
    static double ToDistance(TudeStr* const base, TudeStr* const target);

protected :

private :

    /* �n�����a */
    static const double EARTH_R = 6378.137;

    /* �p�x �� ���W�A�� �ϊ� */
    static double degreeToRadian(const double degree);

    /* ���W�A�� �� �p�x �ϊ� */
    static double radianToDegree(const double radian);
};

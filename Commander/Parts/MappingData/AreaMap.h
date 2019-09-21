#pragma once

#include "MappingData.h"

class AreaMap : public MappingData
{
public :

    static const unsigned long  UNKNOWN_VALUE = 0;      /* ���m�� */
    static const unsigned long  MOVABLE_VALUE = 1;      /* �ړ��\ */
    static const unsigned long  NOT_MOVABLE_VALUE = 2;  /* �ړ��s�� */
    static const unsigned long  OBSTACLE_VALUE = 3;     /* ��Q�� */

    AreaMap();
    virtual ~AreaMap();

    /* �����f�[�^���� */
    void SetInitialData();

    /* �w�肵�����W���ړ��\�����f���� */
    bool IsMovable(const unsigned long x, const unsigned long y);

protected :


private :

};
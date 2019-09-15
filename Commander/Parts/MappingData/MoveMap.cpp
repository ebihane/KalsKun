#include "MoveMap.h"

MoveMap::MoveMap()
 : MappingData((char *)"/home/pi/MoveMap.dat")
 , m_MovedValue(MOVED_INIT_VALUE)
 , m_NotMoveValue(NOT_MOVE_INIT_VALUE)
{
    /* nop. */
}

MoveMap::~MoveMap()
{
    /* nop. */
}

/* �w�肵�����W���ړ��\�����f���� */
bool MoveMap::IsMovable(const unsigned long x, const unsigned long y)
{
    bool retVal = false;

    if (isInRange(x, y) != true)
    {
        goto FINISH;
    }

    if (Get(x, y) == MOVE_FAILED_NO)
    {
        goto FINISH;
    }

    retVal = true;

FINISH :
    return retVal;
}

/* �w�肵�����W�����ړ������f���� */
bool MoveMap::IsNotMove(const unsigned long x, const unsigned long y)
{
    bool retVal = false;

    if (isInRange(x, y) != true)
    {
        /* ���W���͈͊O�̏ꍇ�͈ړ������Ȃ����߂Ɂu�ړ��ς݁v��Ԃ� */
        goto FINISH;
    }

    if (Get(x, y) != m_NotMoveValue)
    {
        goto FINISH;
    }

    retVal = true;

FINISH :
    return retVal;
}

/* ���s�f�[�^���X�V���� */
void MoveMap::UpdateMovedValue()
{
    unsigned long tempValue = m_MovedValue;

    tempValue++;
    if (tempValue == MOVE_FAILED_NO)
    {
        m_MovedValue = MOVED_INIT_VALUE;
        m_NotMoveValue = NOT_MOVE_INIT_VALUE;
    }
    else
    {
        m_NotMoveValue = m_MovedValue;
        m_MovedValue = tempValue;
    }
}

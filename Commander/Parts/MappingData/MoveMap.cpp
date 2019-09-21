#include "Parts/Setting/SettingManager.h"
#include "MoveMap.h"

MoveMap* MoveMap::m_This = new MoveMap();

MoveMap::MoveMap()
 : MappingData((char *)"/home/pi/Information/MoveMap.dat")
 , m_MovedValue(MOVED_INIT_VALUE)
 , m_NotMoveValue(NOT_MOVE_INIT_VALUE)
{
    /* nop. */
}

MoveMap::~MoveMap()
{
    /* nop. */
}

MoveMap* const MoveMap::GetInstance()
{
    return m_This;
}

/* �����f�[�^���� */
void MoveMap::SetInitialData()
{
    SettingManager* setting = SettingManager::GetInstance();
    RectStr mapCount = setting->GetMapCount();
    
    for (long y = 0; y < mapCount.Y; y++)
    {
        for (long x = 0; x < mapCount.X; x++)
        {
            if ((y == 0) || (y == (mapCount.Y - 1)))
            {
                Set(x, y, MOVE_FAILED_NO);
            }
            else if ((x == 0) || (x == (mapCount.X - 1)))
            {
                Set(x, y, MOVE_FAILED_NO);
            }
            else
            {
                Set(x, y, NOT_MOVE_INIT_VALUE);
            }
        }
    }
}

/* �w�肵�����W���ړ��\�����f���� */
bool MoveMap::IsMovable(RectStr* const position)
{
    return IsMovable(position->X, position->Y);
}

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
bool MoveMap::IsNotMove(RectStr* const position)
{
    return IsNotMove(position->X, position->Y);
}

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
#include "Parts/ShareMemory/ShareMemory.h"
#include "Parts/Setting/SettingManager.h"
#include "AreaMap.h"
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
    RectStr mapCount = { 0 };
    setting->GetMapCount(&mapCount);
    
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

    pShareMemory->Commander.MapMovedValue = m_MovedValue;
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
    unsigned char value = 0;

    /* ���W���͈͊O */
    if (isInRange(x, y) != true)
    {
        /* �ړ������Ȃ����߂Ɂu�ړ��ς݁v��Ԃ� */
        goto FINISH;
    }

    value = Get(x, y);

    /* ���s�֎~�G���A */
    if (value == MOVE_FAILED_NO)
    {
        /* �ړ������Ȃ����߂Ɂu�ړ��ς݁v��Ԃ� */
        goto FINISH;
    }

    /* �ړ��ς� */
    if (value == m_MovedValue)
    {
        goto FINISH;
    }

    retVal = true;

FINISH :
    return retVal;
}

/* �w�肵�����W�𑖍s�ς݂ɕύX���� */
void MoveMap::ChangeMoved(RectStr* const position)
{
    ChangeMoved(position->X, position->Y);
}

void MoveMap::ChangeMoved(const unsigned long x, const unsigned long y)
{
    AreaMap* areaMap = AreaMap::GetInstance();

    if (areaMap->IsMovable(x, y) == false)
    {
        goto FINISH;
    }

    Set(x, y, m_MovedValue);

FINISH :
    return;
}

/* ���̑S���𑖍s�������������肷�� */
bool MoveMap::IsComplete()
{
    bool retVal = false;
    SettingManager* setting = SettingManager::GetInstance();
    AreaMap* areaMap = AreaMap::GetInstance();
    float moveEndRate = 0;
    RectStr mapCount = { 0 };
    char areaMapValue = 0;
    char moveMapValue = 0;

    unsigned long movableCount = 0;
    unsigned long movedCount = 0;

    setting->GetMapCount(&mapCount);
    moveEndRate = setting->GetMoveEndRate();

    for (long y = 0; y < mapCount.Y; y++)
    {
        for (long x = 0; x < mapCount.X; x++)
        {
            /* ���s�֎~�̏ꍇ�̓J�E���g���Ȃ� */
            if (areaMap->IsMovable(x, y) == false)
            {
                continue;
            }

            /* ��Q���͖������ɂ��Ȃ� */
            areaMapValue = areaMap->Get(x, y);
            if (areaMapValue == AreaMap::OBSTACLE_VALUE)
            {
                continue;
            }

            /* �ړ��\�G���A�����Z */
            movableCount++;

            /* �ړ����� */
            moveMapValue = Get(x, y);
            if (moveMapValue == m_MovedValue)
            {
                /* �ړ������G���A�����Z */
                movedCount++;
            }
        }
    }

    /* �ړ��\�G���A�ɑ΂��āA�ړ������G���A�̊������ݒ�l�ȏ�̏ꍇ�A�����Ƃ��� */
    float rate = (float)movedCount / (float)movableCount;
    if (moveEndRate <= rate)
    {
        retVal = true;
    }


    return retVal;
}

/* ���s�f�[�^���X�V���� */
void MoveMap::UpdateMovedValue()
{
    unsigned char tempValue = m_MovedValue;

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

    pShareMemory->Commander.MapMovedValue = m_MovedValue;
}

/* �ǂݍ��݌�̏��� */
void MoveMap::loadedProc()
{
    unsigned char areaMapValue = 0;
    unsigned char moveMapValue = 0;
    unsigned char maxValue = 0;
    SettingManager* setting = SettingManager::GetInstance();
    AreaMap* areaMap = AreaMap::GetInstance();

    RectStr mapCount = { 0 };
    setting->GetMapCount(&mapCount);

    /* ���͂ɈӐ}�I�Ɉړ��֎~�G���A��ݒ肵�Ă��� */
    for (long y = 1; y < mapCount.Y - 1; y++)
    {
        for (long x = 1; x < mapCount.X - 1; x++)
        {
            areaMapValue = areaMap->Get(x, y);
            moveMapValue = Get(x, y);

            /* �ړ��\�ȊO */
            if (areaMapValue != AreaMap::MOVABLE_VALUE)
            {
                continue;
            }

            if (moveMapValue < maxValue)
            {
                continue;
            }

            maxValue = moveMapValue;
        }
    }

    m_MovedValue = maxValue;
    m_NotMoveValue = (unsigned char)(maxValue - 1);
    pShareMemory->Commander.MapMovedValue = m_MovedValue;
}

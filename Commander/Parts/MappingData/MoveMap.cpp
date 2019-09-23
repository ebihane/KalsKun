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

/* 初期データ生成 */
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
}

/* 指定した座標が移動可能か判断する */
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

/* 指定した座標が未移動か判断する */
bool MoveMap::IsNotMove(RectStr* const position)
{
    return IsNotMove(position->X, position->Y);
}

bool MoveMap::IsNotMove(const unsigned long x, const unsigned long y)
{
    bool retVal = false;

    if (isInRange(x, y) != true)
    {
        /* 座標が範囲外の場合は移動させないために「移動済み」を返す */
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

/* 指定した座標を走行済みに変更する */
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

/* 畑の全区画を走行完了したか判定する */
bool MoveMap::IsComplete()
{
    bool retVal = true;
    SettingManager* setting = SettingManager::GetInstance();
    AreaMap* areaMap = AreaMap::GetInstance();
    RectStr mapCount = { 0 };
    char areaMapValue = 0;
    char moveMapValue = 0;

    setting->GetMapCount(&mapCount);

    for (long y = 0; y < mapCount.Y; y++)
    {
        for (long x = 0; x < mapCount.X; x++)
        {
            /* 走行禁止の場合は未完了にしない */
            if (areaMap->IsMovable(x, y) == false)
            {
                continue;
            }

            /* 障害物は未完了にしない */
            areaMapValue = areaMap->Get(x, y);
            if (areaMapValue == AreaMap::OBSTACLE_VALUE)
            {
                continue;
            }

            moveMapValue = Get(x, y);
            if (moveMapValue != m_MovedValue)
            {
                retVal = false;
                goto FINISH;
            }
        }
    }

FINISH :
    return retVal;
}

/* 走行データを更新する */
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
}

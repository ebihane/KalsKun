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

    pShareMemory->Commander.MapMovedValue = m_MovedValue;
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
    unsigned char value = 0;

    /* 座標が範囲外 */
    if (isInRange(x, y) != true)
    {
        /* 移動させないために「移動済み」を返す */
        goto FINISH;
    }

    value = Get(x, y);

    /* 走行禁止エリア */
    if (value == MOVE_FAILED_NO)
    {
        /* 移動させないために「移動済み」を返す */
        goto FINISH;
    }

    /* 移動済み */
    if (value == m_MovedValue)
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
            /* 走行禁止の場合はカウントしない */
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

            /* 移動可能エリア数加算 */
            movableCount++;

            /* 移動完了 */
            moveMapValue = Get(x, y);
            if (moveMapValue == m_MovedValue)
            {
                /* 移動完了エリア数加算 */
                movedCount++;
            }
        }
    }

    /* 移動可能エリアに対して、移動完了エリアの割合が設定値以上の場合、完了とする */
    float rate = (float)movedCount / (float)movableCount;
    if (moveEndRate <= rate)
    {
        retVal = true;
    }


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

    pShareMemory->Commander.MapMovedValue = m_MovedValue;
}

/* 読み込み後の処理 */
void MoveMap::loadedProc()
{
    unsigned char areaMapValue = 0;
    unsigned char moveMapValue = 0;
    unsigned char maxValue = 0;
    SettingManager* setting = SettingManager::GetInstance();
    AreaMap* areaMap = AreaMap::GetInstance();

    RectStr mapCount = { 0 };
    setting->GetMapCount(&mapCount);

    /* 周囲に意図的に移動禁止エリアを設定している */
    for (long y = 1; y < mapCount.Y - 1; y++)
    {
        for (long x = 1; x < mapCount.X - 1; x++)
        {
            areaMapValue = areaMap->Get(x, y);
            moveMapValue = Get(x, y);

            /* 移動可能以外 */
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

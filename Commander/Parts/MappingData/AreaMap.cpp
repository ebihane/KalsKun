#include "Parts/Setting/SettingManager.h"
#include "AreaMap.h"

AreaMap* const AreaMap::m_This = new AreaMap();

AreaMap::AreaMap()
 : MappingData((char*)"/home/pi/Information/AreaMap.dat")
{
    /* nop. */
}

AreaMap::~AreaMap()
{
    /* nop. */
}

AreaMap* const AreaMap::GetInstance()
{
    return m_This;
}

/* 初期データ生成 */
void AreaMap::SetInitialData()
{
    SettingManager* setting = SettingManager::GetInstance();
    RectStr mapCount = setting->GetMapCount();

    for (long y = 0; y < mapCount.Y; y++)
    {
        for (long x = 0; x < mapCount.X; x++)
        {
            if ((y == 0) || (y == (mapCount.Y - 1)))
            {
                Set(x, y, NOT_MOVABLE_VALUE);
            }
            else if ((x == 0) || (x == (mapCount.X - 1)))
            {
                Set(x, y, NOT_MOVABLE_VALUE);
            }
            else
            {
                Set(x, y, MOVABLE_VALUE);
            }
        }
    }
}

/* 指定した座標が移動可能か判断する */
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

#include <string.h>
#include "PositionData.h"

/* 向いている方向をもとに、前進した場合の座標を算出するためのテーブル */
/* マップの [0][0] が北西方向としている */
const PositionData::RectStr PositionData::m_ReferenceMap[CompassEnum::E_COMPASS_MAX]
= {
    /*   X       Y      */
    /*-------+----------*/
    {    0,     -1  },  /* 北 */
    {    1,     -1  },  /* 北東 */
    {    1,      0  },  /* 東 */
    {    1,      1  },  /* 南東 */
    {    0,      1  },  /* 南 */
    {   -1,      1  },  /* 南西 */
    {   -1,      0  },  /* 西 */
    {   -1,     -1  },  /* 北西 */
};

PositionData::PositionData(const CompassEnum compass, RectStr* const position)
 : m_Compass(compass)
{
    memcpy(&m_Position, position, sizeof(RectStr));
}

PositionData::~PositionData()
{
    /* nop. */
}

void PositionData::GetPosition(RectStr* const position)
{
    memcpy(position, &m_Position, sizeof(RectStr));
}

CompassEnum PositionData::GetCompass()
{
    return m_Compass;
}

/* 向いている方角に進んだ時の座標を取得する */
void PositionData::GetNextPosition(RectStr* const position)
{
    position->X = m_Position.X + m_ReferenceMap[m_Compass].X;
    position->Y = m_Position.Y + m_ReferenceMap[m_Compass].Y;
}

/* 指定した方向に進んだ時の座標を取得する */
void PositionData::GetNextPosition(const DirectionEnum direction, RectStr* const position)
{
    CompassEnum nextCompass = convertDirectionToCompass(m_Compass, direction);
    position->X = m_Position.X + m_ReferenceMap[nextCompass].X;
    position->Y = m_Position.X + m_ReferenceMap[nextCompass].Y;
}

/* 向いている方向を変える */
void PositionData::SetDirection(DirectionEnum const direction)
{
    m_Compass = convertDirectionToCompass(m_Compass, direction);
}

/* 向いている方向に進む */
void PositionData::Move()
{
    m_Position.X += m_ReferenceMap[m_Compass].X;
    m_Position.Y += m_ReferenceMap[m_Compass].Y;
}

/* 指定された方角に向きを加味した新たな方角を返す */
CompassEnum PositionData::convertDirectionToCompass(const CompassEnum compass, const DirectionEnum direction)
{
    CompassEnum retVal = CompassEnum::E_COMPASS_MAX;
    long tempCompass = (long)compass;

    tempCompass += (long)direction;
    tempCompass %= (long)CompassEnum::E_COMPASS_MAX;
    retVal = (CompassEnum)tempCompass;

    return retVal;
}

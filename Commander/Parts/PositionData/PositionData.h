#pragma once

#include "Include/Common.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Parts/Setting/SettingManager.h"
#include "Parts/MappingData/AreaMap.h"
#include "Parts/MappingData/MoveMap.h"

/* 方向と方角を管理するためのクラス */
/* 上位層では「前後左右」で指示を出したいが、ロボットの位置は絶対的なデータ (= 方角) で判断したいため、このクラスを噛ませる */
/* なお、本クラスで取得できる座標は、「畑の範囲内か」「走行禁止か」といった判断は度外視している点に注意が必要 */
/*  方向：「前後左右」を基準とした、ロボットの向きの情報。ロボットの姿勢によって参照する方角が変わる */
/*  方角：「東西南北」を基準とした、ロボットが向いている方角の情報。*/
class PositionData
{
public :

    /* 方角定数 */
    typedef enum
    {
        E_COMPASS_NORTH = 0,    /* 0: 北 */
        E_COMPASS_NORTHEAST,    /* 1: 北東 */
        E_COMPASS_EAST,         /* 2: 東 */
        E_COMPASS_SOUTHEAST,    /* 3: 南東 */
        E_COMPASS_SOUTH,        /* 4: 南 */
        E_COMPASS_SOUTHWEST,    /* 5: 南西 */
        E_COMPASS_WEST,         /* 6: 西 */
        E_COMPASS_NORTHWEST,    /* 7: 北西 */
        E_COMPASS_MAX,          /* 8: 方角数 */
    } CompassEnum;

    PositionData(const CompassEnum compass, RectStr* const position);
    virtual ~PositionData();

    /* 現在の座標を取得する */
    void GetPosition(RectStr* const position);

    /* 現在向いている方角を取得する */
    CompassEnum GetCompass();

    /* 向いている方角に進んだ時の座標を取得する */
    void GetNextPosition(RectStr* const position);

    /* 指定した方向に進んだ時の座標を取得する */
    void GetNextPosition(const DirectionEnum direction, RectStr* const position);

    /* 向いている方向を変える */
    void SetDirection(DirectionEnum const direction);

    /* 向いている方向に進む */
    void Move();

protected :

private :

    static const RectStr m_ReferenceMap[CompassEnum::E_COMPASS_MAX];    /* 座標移動方向算出マップ */

    RectStr     m_Position;     /* 現在位置 */
    CompassEnum m_Compass;      /* 現在向いている方角 */

    /* 指定された方角に向きを加味した新たな方角を返す */
    CompassEnum convertDirectionToCompass(const CompassEnum compass, const DirectionEnum direction);

};

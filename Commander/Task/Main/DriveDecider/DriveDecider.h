#pragma once

#include "Logger/Logger.h"
#include "Measure/Stopwatch.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Parts/Setting/SettingManager.h"
#include "Parts/MappingData/AreaMap.h"
#include "Parts/MappingData/MoveMap.h"
#include "Parts/PositionData/PositionData.h"

class DriveDecider
{
public :

    DriveDecider();
    virtual ~DriveDecider();

    /* 動作決定 (通常時) */
    virtual MotorCommandEnum Decide();

    /* 動作決定 (通行禁止エリア検知時) */
    virtual MotorCommandEnum DecideForRoadClosed();

    /* 動作決定 (障害物検知時) */
    virtual MotorCommandEnum DecideForAvoidance();

    /* 網羅完了判定 */
    virtual bool IsComplete();

    /* ターン回数取得 */
    unsigned long GetTurnCount();

protected :

    Logger m_Logger;            /* Log Accessor */
    AreaMap* m_AreaMap;         /* エリアマップクラスインスタンス */
    MoveMap* m_MoveMap;         /* 動作マップクラスインスタンス */
    PositionData* m_Position;   /* 位置情報管理クラスインスタンス  */
    SettingManager* m_Setting;  /* 設定管理クラスインスタンス */
    Stopwatch m_FrontKeepWatch; /* ターン時前進キープ時間計測用ストップウォッチ */
    MotorCommandEnum m_Preview; /* 直前の決定内容 */
    RectStr m_MapCount;         /* 畑のサイズ */

    unsigned long m_TurnCount;  /* ターン回数 */

private :

};

#pragma once

#include "Include/Common.h"
#include "Logger/Logger.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Parts/Setting/SettingManager.h"
#include "Parts/MappingData/AreaMap.h"
#include "Parts/MappingData/MoveMap.h"

/* 方向と方角を管理するためのシングルトンクラス */
/* 上位層では「前後左右」で指示を出したいが、ロボットの位置は絶対的なデータ (= 方角) で判断したいため、このクラスを噛ませる */
/*  方向：「前後左右」を基準とした、ロボットの向きの情報。ロボットの姿勢によって参照する方角が変わる */
/*  方角：「東西南北」を基準とした、ロボットが向いている方角の情報。*/
class PositionData
{
public :

    static const int    ERROR_NOTHING = 0;

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

    /* インスタンスを取得する */
    static PositionData* const GetInstance();

    /* 現在の座標をセットする */
    void SetPosition(RectStr* const position);

    /* 現在の座標を取得する */
    RectStr GetPosition(void);

    /* 現在向いている方角を取得する */
    CompassEnum GetCompass();

    /* 向いている方角に進んだ時の座標を取得する */
    void GetNextPosition(RectStr* const position);

    /* 指定した方向に進んだ時の座標を取得する */
    void GetNextPosition(const DirectionEnum direction, RectStr* const position);

    /* 向いている方向を変える */
    void SetDirection(DirectionEnum const direction);

    /*-----------------*/
    /* 初期化用 処理群 */
    /*-----------------*/
    /* ファイルがあるか確認する */
    bool IsFileExist();

    /* 位置情報と向いている方角をファイルに保存する */
    ResultEnum Save();

    /* 位置情報と向いている方角を読み込む */
    ResultEnum Load();

    /* 初期データ生成 */
    void SetInitialData();

    /* 最後に発生したシステムエラーを取得する */
    int GetLastError();

protected :

private :

    static const RectStr m_ReferenceMap[CompassEnum::E_COMPASS_MAX];    /* 座標移動方向算出マップ */

    static PositionData* const m_This;  /* 自クラスインスタンス */

    Logger      m_Logger;               /* Logger */
    RectStr     m_Position;             /* 現在位置 */
    RectStr     m_PrevPosition;         /* 直前の位置 */
    CompassEnum m_Compass;              /* 現在向いている方角 */
    int         m_LastErrorNo;          /* 最後に発生したシステムエラー番号 */
    char        m_LogStr[LOG_OUT_MAX];  /* ログ出力バッファ */

    PositionData();
    virtual ~PositionData();

    /* 指定された方角に向きを加味した新たな方角を返す */
    CompassEnum convertDirectionToCompass(const CompassEnum compass, const DirectionEnum direction);

    /* 文字列を特定の文字で分割する */
    void parseString(char* const src, char* const dest, const char delimiter, const long maxLen);

};

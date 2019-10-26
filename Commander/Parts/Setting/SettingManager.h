#pragma once

#include "Include/Common.h"

class SettingManager
{
public :

    /* 曜日定数 */
    typedef enum
    {
        Sunday = 0,     /* 0: 日曜日 */
        Monday,         /* 1: 月曜日 */
        Tuesday,        /* 2: 火曜日 */
        Wednesday,      /* 3: 水曜日 */
        Tharsday,       /* 4: 木曜日 */
        Friday,         /* 5: 金曜日 */
        Saturday,       /* 6: 土曜日 */
        Everyday,       /* 7: 毎日 */
        Nothing,        /* 8: 無し */
    } DayOfWeekEnum;

    /* 開始日時 */
    typedef struct
    {
        DayOfWeekEnum   DayOfWeek;
        unsigned char   Hour;
        unsigned char   Minute;
    } TimeSettingStr;

    /* インスタンス取得 */
    static SettingManager* const GetInstance();

    /* デフォルトデータ生成 */
    void CreateDefaultData();

    /* ファイル操作 */
    bool IsFileExist();     /* ファイル有無確認 */
    ResultEnum Save();      /* 保存 */
    ResultEnum Load();      /* 読み込み */
    int GetLastErrorNo();   /* エラー番号取得 */

    /*--------------------*/
    /* 設定値の取得・変更 */
    /*--------------------*/
    /* ロボットサイズ */
    void GetRobotSize(SizeStr* const size);     /* 取得 */
    void SetRobotSize(SizeStr* const size);     /* 設定 */

    /* 畑のサイズ */
    void GetFarmSize(SizeStr* const size);      /* 取得 */
    void SetFarmSize(SizeStr* const size);      /* 設定 */

    /* マップの個数 */
    void GetMapCount(RectStr* const count);     /* 取得 */
    void UpdateMapCount();                      /* 再計算 */

    /* 電波の伝搬係数 */
    float GetWavePowerCoeff();                  /* 取得 */
    void SetWavePowerCoeff(const float coeff);  /* 設定 */

    /* 草刈り開始時刻 */
    TimeSettingStr GetKusakariStartTime();                  /* 取得 */
    void SetKusakariStartTime(TimeSettingStr* const date);  /* 設定 */

    /* 夜警開始時刻 */
    TimeSettingStr GetYakeiStartTime();                     /* 取得 */
    void SetYakeStartTime(TimeSettingStr* const date);      /* 設定 */

    /* アクセスポイント */
    long GetApCount();                          /* 個数 取得 */
    void GetApAddress(RectStr* data);           /* 座標 取得 */
    void SetApInfo(const long count, RectStr* address); /* 個数＋座標 設定 */

protected :


private :

    static SettingManager* m_This;  /* 自クラスインスタンス */

    int             m_LastErrorNo;      /* 最後に発生したシステムエラー */

    SizeStr         m_RobotSize;        /* ロボットサイズ */
    SizeStr         m_FarmSize;         /* 畑のサイズ */
    RectStr         m_MapCount;         /* マップの個数 */
    float           m_WavePowerCoeff;   /* 電波の伝搬係数 */
    TimeSettingStr  m_KusakariStart;    /* 草刈り開始時刻情報 */
    TimeSettingStr  m_YakeiStart;       /* 夜警開始時刻情報 */
    long            m_ApCount;          /* アクセスポイントの個数 */
    RectStr*        m_ApAddress;        /* アクセスポイントの座標 */

    SettingManager();
    virtual ~SettingManager();

    long calcurateSettingSize();    /* 設定の合計サイズ取得 */
    void parseString(char* const src, char* const dest, const char delimiter, const long maxLen);

};

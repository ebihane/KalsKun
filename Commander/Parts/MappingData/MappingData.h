#pragma once

#include "Include/Common.h"

class MappingData
{
public:

    static const int            ERROR_NOTHING = 0;
    static const unsigned char  OUT_OF_AREA_VALUE = 0xFF;

    MappingData(char* const path);
    virtual ~MappingData();

    /* 指定した座標のデータを取得する (-1 の場合、範囲外) */
    unsigned char Get(RectStr* position);
    unsigned char Get(const long x, const long y);

    /* 指定した座標周辺 ([1, 1]を中心とした8方向) のデータを取得する */
    void Get(RectStr* position, unsigned char** const value);
    void Get(const long x, const long y, unsigned char** const value);

    /* 指定した座標のデータを変更する */
    ResultEnum Set(RectStr* const position, const unsigned char value);
    ResultEnum Set(const long x, const long y, const unsigned char value);

    /* 最後に発生したシステムエラーを取得する */
    int GetLastError();

    /* マップの状態を表示する */
    void Print();

    /*-----------------*/
    /* 初期化用 処理群 */
    /*-----------------*/
    /* ファイルがあるか確認する */
    bool IsFileExist();

    /* Mapping Data を確保する */
    ResultEnum Allocate();

    /* Mapping Data を解放する */
    void Free();

    /* Mapping Data の領域サイズと現在の値をファイルに保存する */
    ResultEnum Save();

    /* Mapping Data の領域サイズと保存時の値をファイルから取得し、MappingData を復元する */
    ResultEnum Load();

    /* 初期データ生成 */
    virtual void SetInitialData() = 0;

protected :

    /* 指定された座標がマップデータの領域内か確認する */
    bool isInRange(const long x, const long y);

private :

    char            m_FilePath[80]; /* ファイルパス */
    unsigned char** m_MapData;      /* マップデータ */
    int             m_LastErrorNo;  /* 最後に発生したシステムエラー番号 */

};
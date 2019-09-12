#pragma once

#include "Include/Common.h"
#include "Parts/ShareMemory/ShareMemory.h"

class MappingData
{
public:

    static const int            ERROR_NOTHING = 0;
    static const unsigned char  OUT_OF_AREA_VALUE = 0xFF;

    MappingData(char* const path);
    virtual ~MappingData();

    /* 指定した座標のデータを取得する (-1 の場合、範囲外) */
    unsigned char Get(const unsigned long x, const unsigned long y);

    /* 指定した座標周辺 ([1, 1]を中心とした8方向) のデータを取得する */
    void Get(const unsigned long x, const unsigned long y, unsigned char** const value);

    /* 指定した座標のデータを変更する */
    ResultEnum Set(const unsigned long x, const unsigned long y, const unsigned char value);

    /* 最後に発生したシステムエラーを取得する */
    int GetLastError();

    /*-----------------*/
    /* 初期化用 処理群 */
    /*-----------------*/
    /* Mapping Data を確保する */
    ResultEnum Allocate(const unsigned long width, const unsigned long height);

    /* Mapping Data を解放する */
    void Free();

    /* Mapping Data の領域サイズと現在の値をファイルに保存する */
    ResultEnum Save();

    /* Mapping Data の領域サイズと保存時の値をファイルから取得し、MappingData を復元する */
    ResultEnum Load();



protected :

private :

    static const unsigned char  PointXArray[3][3];
    static const unsigned char  PointYArray[3][3];

    char            m_FilePath[80]; /* ファイルパス */

    unsigned long   m_Width;        /* 幅 (= X 軸) */
    unsigned long   m_Height;       /* 高さ (= Y 軸) */
    unsigned char** m_MapData;      /* マップデータ */

    int             m_LastErrorNo;  /* 最後に発生したシステムエラー番号 */

    /* 指定された座標がマップデータの領域内か確認する */
    bool isInRange(const unsigned long x, const unsigned long y);

};
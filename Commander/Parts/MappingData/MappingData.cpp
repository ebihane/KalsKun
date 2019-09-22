#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "Parts/Setting/SettingManager.h"
#include "MappingData.h"

MappingData::MappingData(char* const path)
 : m_MapData(NULL)
 , m_LastErrorNo(ERROR_NOTHING)
{
    strncpy(&m_FilePath[0], &path[0], sizeof(m_FilePath));
}

MappingData::~MappingData()
{
    /* nop. */
}

/* 指定した座標のデータを取得する (-1 の場合、範囲外) */
unsigned char MappingData::Get(RectStr* position)
{
    return Get(position->X, position->Y);
}

unsigned char MappingData::Get(const long x, const long y)
{
    unsigned char retVal = 0;

    /* 座標が範囲外 */
    if (isInRange(x, y) != true)
    {
        retVal = OUT_OF_AREA_VALUE;
    }
    else
    {
        retVal = m_MapData[y][x];
    }

    return retVal;
}

/* 指定した座標周辺 ([1, 1]を中心とした8方向) のデータを取得する (-1 は範囲外) */
void MappingData::Get(RectStr* position, unsigned char** const value)
{
    Get(position->X, position->Y, value);
}

void MappingData::Get(const long x, const long y, unsigned char** const value)
{
    unsigned char   posX = 0;
    unsigned char   posY = 0;
    long            offsetX = 0;
    long            offsetY = 0;

    for (offsetY = 1; -1 <= offsetY; offsetY--)
    {
        for (offsetX = -1; offsetX <= 1; offsetX++)
        {
            if (isInRange(x + offsetX, y + offsetY) != true)
            {
                value[posY][posX] = OUT_OF_AREA_VALUE;
            }
            else
            {
                value[posY][posX] = m_MapData[posY][posX];
            }

            posX++;
        }

        posX = 0;
        posY++;
    }
}

/* 指定した座標のデータを変更する */
ResultEnum MappingData::Set(RectStr* const position, const unsigned char value)
{
    return Set(position->X, position->Y, value);
}

ResultEnum MappingData::Set(const long x, const long y, const unsigned char value)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    /* 座標が範囲外 */
    if (isInRange(x, y) != true)
    {
        goto FINISH;
    }

    /* 範囲外データはセット不可 */
    if (value == OUT_OF_AREA_VALUE)
    {
        goto FINISH;
    }

    m_MapData[y][x] = value;

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

/* 最後に発生したシステムエラーを取得する */
int MappingData::GetLastError()
{
    return m_LastErrorNo;
}

/*-----------------*/
/* 初期化用 処理群 */
/*-----------------*/
/* ファイルがあるか確認する */
bool MappingData::IsFileExist()
{
    bool retVal = false;
    FILE* fp = NULL;

    
    /* 読み取り属性で開けるか確認 */
    fp = fopen(&m_FilePath[0], "rb");
    if (fp == NULL)
    {
        /* 開けない場合は存在しないものとする */
        goto FINISH;
    }

    fclose(fp);

    retVal = true;

FINISH :
    return retVal;
}

/* Mapping Data を確保する */
ResultEnum MappingData::Allocate()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    long y = 0;
    SettingManager* setting = SettingManager::GetInstance();
    RectStr mapCount = { 0 };
    setting->GetMapCount(&mapCount);

    if (m_MapData != NULL)
    {
        retVal = ResultEnum::NormalEnd;
        goto FINISH;
    }

    m_MapData = (unsigned char **)malloc(sizeof(unsigned char*) * mapCount.Y);
    if (m_MapData == NULL)
    {
        goto FINISH;
    }

    for (y = 0; y < mapCount.Y; y++)
    {
        m_MapData[y] = (unsigned char*)malloc(sizeof(unsigned char) * mapCount.X);
        if (m_MapData[y] == NULL)
        {
            goto FINISH;
        }

        memset(&m_MapData[y][0], 0x00, sizeof(unsigned char) * mapCount.X);
    }

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

/* Mapping Data を解放する */
void MappingData::Free()
{
    long y = 0;
    SettingManager* setting = SettingManager::GetInstance();
    RectStr mapCount = { 0 };
    setting->GetMapCount(&mapCount);

    if (m_MapData == NULL)
    {
        goto FINISH;
    }

    for (y = 0; y < mapCount.Y; y++)
    {
        free(m_MapData[y]);
    }

    free(m_MapData);
    m_MapData = NULL;


FINISH:
    return;
}

/* Mapping Data の領域サイズと現在の値をファイルに保存する */
ResultEnum MappingData::Save()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    FILE* fp = NULL;
    SettingManager* setting = SettingManager::GetInstance();
    RectStr mapCount = { 0 };
    setting->GetMapCount(&mapCount);

    /* MapData が存在しない */
    if (m_MapData == NULL)
    {
        goto FINISH;
    }

    m_LastErrorNo = ERROR_NOTHING;

    /* ファイルオープン */
    fp = fopen(&m_FilePath[0], "wb");
    if (fp == NULL)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    for (long y = 0; y < mapCount.Y; y++)
    {
        for (long x = 0; x < mapCount.X; x++)
        {
            /* マップデータ 書き込み */
            if (fwrite(&m_MapData[y][x], 1, 1, fp) < 0)
            {
                m_LastErrorNo = errno;
                goto FINISH;
            }
        }
    }

    retVal = ResultEnum::NormalEnd;

    
FINISH :

    if (fp != NULL)
    {
        if (fclose(fp) < 0)
        {
            m_LastErrorNo = errno;
            retVal = ResultEnum::AbnormalEnd;
        }
    }

    return retVal;
}

/* Mapping Data の領域サイズと保存時の値をファイルから取得し、MappingData を復元する */
ResultEnum MappingData::Load()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    FILE* fp = NULL;
    SettingManager* setting = SettingManager::GetInstance();
    RectStr mapCount = { 0 };
    setting->GetMapCount(&mapCount);

    if (m_MapData != NULL)
    {
        goto FINISH;
    }

    m_LastErrorNo = 0;

    /* ファイルオープン */
    fp = fopen(&m_FilePath[0], "rb");
    if (fp == NULL)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* 領域確保 */
    if (Allocate() != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    for (long y = 0; y < mapCount.Y; y++)
    {
        for (long x = 0; x < mapCount.X; x++)
        {
            /* マップデータ 読み込み */
            if (fread(&m_MapData[y][x], 1, 1, fp) < 0)
            {
                m_LastErrorNo = errno;
                goto FINISH;
            }
        }
    }

    retVal = ResultEnum::NormalEnd;

FINISH :

    if (fp != NULL)
    {
        if (fclose(fp) < 0)
        {
            m_LastErrorNo = errno;
            retVal = ResultEnum::AbnormalEnd;
        }
    }

    return retVal;
}

/* 指定された座標がマップデータの領域内か確認する */
bool MappingData::isInRange(const long x, const long y)
{
    bool retVal = false;
    SettingManager* setting = SettingManager::GetInstance();
    RectStr mapCount = { 0 };
    setting->GetMapCount(&mapCount);

    if ((x < 0) || (mapCount.X <= x))
    {
        goto FINISH;
    }

    if ((y < 0) || (mapCount.Y <= y))
    {
        goto FINISH;
    }

    retVal = true;

FINISH :
    return retVal;
}

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "MappingData.h"

MappingData::MappingData(char* const path)
 : m_Width(0)
 , m_Height(0)
 , m_MapData(NULL)
 , m_LastErrorNo(ERROR_NOTHING)
{
    strncpy(&m_FilePath[0], &path[0], sizeof(m_FilePath));
}

MappingData::~MappingData()
{
    /* nop. */
}

/* 指定した座標のデータを取得する (-1 の場合、範囲外) */
unsigned char MappingData::Get(const unsigned long x, const unsigned long y)
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
void MappingData::Get(const unsigned long x, const unsigned long y, unsigned char** const value)
{
    unsigned char   posX = 0;
    unsigned char   posY = 0;
    unsigned long   offsetX = 0;
    unsigned long   offsetY = 0;

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
ResultEnum MappingData::Set(const unsigned long x, const unsigned long y, const unsigned char value)
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
/* Mapping Data を確保する */
ResultEnum MappingData::Allocate(const unsigned long width, const unsigned long height)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    unsigned long y = 0;
    unsigned long* matrix = NULL;
    unsigned long* baseMatrix = NULL;

    if (m_MapData != NULL)
    {
        retVal = ResultEnum::NormalEnd;
        goto FINISH;
    }

    m_MapData = new unsigned char*[height];
    if (m_MapData == NULL)
    {
        goto FINISH;
    }

    for (y = 0; y < height; y++)
    {
        m_MapData[y] = new unsigned char[width];
        if (m_MapData[y] == NULL)
        {
            goto FINISH;
        }

        memset(&m_MapData[y][0], 0x00, sizeof(unsigned char) * width);
    }

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

/* Mapping Data を解放する */
void MappingData::Free()
{
    unsigned long y = 0;

    if (m_MapData == NULL)
    {
        goto FINISH;
    }

    for (y = 0; y < m_Width; y++)
    {
        delete[] m_MapData[y];
    }

    delete[] m_MapData;
    m_MapData = NULL;


FINISH:
    return;
}

/* Mapping Data の領域サイズと現在の値をファイルに保存する */
ResultEnum MappingData::Save()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    int fd = -1;

    /* MapData が存在しない */
    if (m_MapData == NULL)
    {
        goto FINISH;
    }

    m_LastErrorNo = 0;

    /* ファイルオープン */
    fd = open(&m_FilePath[0], O_RDWR | O_CREAT, 0666);
    if (fd < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* 幅 書き込み */
    if (write(fd, &m_Width, sizeof(m_Width)) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* 高さ 書き込み */
    if (write(fd, &m_Height, sizeof(m_Height)) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* マップデータ 書き込み */
    if (write(fd, &m_MapData[0][0], sizeof(unsigned char) * m_Width * m_Height) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;

    
FINISH :

    if (0 <= fd)
    {
        if (close(fd) < 0)
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
    int fd = -1;
    unsigned long width = 0;
    unsigned long height = 0;

    if (m_MapData != NULL)
    {
        goto FINISH;
    }

    m_LastErrorNo = 0;

    /* ファイルオープン */
    fd = open(&m_FilePath[0], O_RDONLY);
    if (fd < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* 幅 読み込み */
    if (read(fd, &width, sizeof(width)) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* 高さ 読み込み */
    if (read(fd, &height, sizeof(height)) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* 領域確保 */
    if (Allocate(width, height) != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    /* マップデータ 読み込み */
    if (read(fd, &m_MapData[0][0], sizeof(unsigned char) * width * height) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    m_Width = width;
    m_Height = height;

    retVal = ResultEnum::NormalEnd;

FINISH :

    if (0 <= fd)
    {
        if (close(fd) < 0)
        {
            m_LastErrorNo = errno;
            retVal = ResultEnum::AbnormalEnd;
        }
    }

    return retVal;
}

/* 指定された座標がマップデータの領域内か確認する */
bool MappingData::isInRange(const unsigned long x, const unsigned long y)
{
    bool retVal = false;

    if ((x < 0) || (m_Width <= x))
    {
        goto FINISH;
    }

    if ((y < 0) || (m_Height <= y))
    {
        goto FINISH;
    }

    retVal = true;

FINISH :
    return retVal;
}

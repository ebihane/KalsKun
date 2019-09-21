#define FILETYPE_TEXT

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "SettingManager.h"

#ifdef FILETYPE_TEXT
    #define FILE_PATH   ("/home/pi/Information/Setting.txt")
#else
    #define FILE_PATH   ("/home/pi/Information/Setting.dat")
#endif

SettingManager* SettingManager::m_This = new SettingManager();

SettingManager::SettingManager()
 : m_LastErrorNo(0)
 , m_ApAddress(NULL)
{
    /* nop. */
}

SettingManager::~SettingManager()
{
    if (m_ApAddress != NULL)
    {
        delete[] m_ApAddress;
        m_ApAddress = NULL;
    }
}

SettingManager* const SettingManager::GetInstance()
{
    return m_This;
}

void SettingManager::CreateDefaultData()
{
    /* ロボットサイズ (全長/全幅/全高) : 600mm * 463mm * 418mm */
    m_RobotSize.Length = 600.0;
    m_RobotSize.Width = 463.0;

    /* 畑の大きさ */
    m_FarmSize.Length = 3000.0;
    m_FarmSize.Width = 3000.0;

    /* マップの個数 (= 自動計算) */
    long xCount = (long)(m_FarmSize.Width / m_RobotSize.Width);
    long yCount = (long)(m_FarmSize.Length / m_RobotSize.Length);

    m_MapCount.X = (xCount * 2) + 2;
    m_MapCount.Y = (yCount * 2) + 2;

    /*  電波の伝搬係数*/
    m_WavePowerCoeff = 2.0f;

    /* アクセスポイントの個数 */
    m_ApCount = 3;

    /* アクセスポイントの座標 */
    if (m_ApAddress != NULL)
    {
        delete[] m_ApAddress;
        m_ApAddress = NULL;
    }

    m_ApAddress = new RectStr[m_ApCount];
    m_ApAddress[0].X = 1;
    m_ApAddress[0].Y = 5;
    m_ApAddress[1].X = 5;
    m_ApAddress[1].Y = 1;
    m_ApAddress[2].X = 8;
    m_ApAddress[2].Y = 9;

    /* 保存する */
    Save();
}

/* ファイル有無確認 */
bool SettingManager::IsFileExist()
{
    bool retVal = false;
    FILE* fp = NULL;


    /* 読み取り属性で開けるか確認 */
    fp = fopen(&FILE_PATH[0], "rb");
    if (fp == NULL)
    {
        /* 開けない場合は存在しないものとする */
        goto FINISH;
    }

    fclose(fp);

    retVal = true;

FINISH:
    return retVal;
}

/* 保存 */
ResultEnum SettingManager::Save()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    FILE* fp = NULL;
    char* buffer = NULL;
    long size = 0;
    long index = 0;

    m_LastErrorNo = 0;

#ifdef FILETYPE_TEXT

    /* ファイルを開く */
    fp = fopen(&FILE_PATH[0], "w");
    if (fp == NULL)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* 書き込み */
    fprintf(fp, "RobotLength,%lf\n", m_RobotSize.Length);
    fprintf(fp, "RobotWidth,%lf\n", m_RobotSize.Width);
    fprintf(fp, "FarmLength,%lf\n", m_FarmSize.Length);
    fprintf(fp, "FarmWidth,%lf\n", m_FarmSize.Width);
    fprintf(fp, "MapCount(X),%ld\n", m_MapCount.X);
    fprintf(fp, "MapCount(Y),%ld\n", m_MapCount.Y);
    fprintf(fp, "WavePowerCoeff,%lf\n", m_WavePowerCoeff);
    fprintf(fp, "APCount,%ld\n", m_ApCount);
    for (index = 0; index < m_ApCount; index++)
    {
        fprintf(fp, "AP%ld(X),%d\n", index + 1, m_ApAddress[index].X);
        fprintf(fp, "AP%ld(Y),%d\n", index + 1, m_ApAddress[index].Y);
    }

#else

    /* ファイルを開く */
    fp = fopen(&FILE_PATH[0], "wb");
    if (fp == NULL)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* 保存サイズの合計を算出 */
    size = calcurateSettingSize();

    /* 保存用領域確保 */
    buffer = new char[size];
    if (buffer == NULL)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* 保存データ作成 */
    memcpy(&buffer[index], &m_RobotSize, sizeof(m_RobotSize));                  index += sizeof(m_RobotSize);
    memcpy(&buffer[index], &m_FarmSize, sizeof(m_FarmSize));                    index += sizeof(m_FarmSize);
    memcpy(&buffer[index], &m_MapCount, sizeof(m_MapCount));                    index += sizeof(m_MapCount);
    memcpy(&buffer[index], &m_WavePowerCoeff, sizeof(m_WavePowerCoeff));        index += sizeof(m_WavePowerCoeff);
    memcpy(&buffer[index], &m_ApCount, sizeof(m_ApCount));                      index += sizeof(m_ApCount);
    memcpy(&buffer[index], m_ApAddress, sizeof(m_ApAddress[0]) * m_ApCount);    index += sizeof(m_ApAddress[0]) * m_ApCount;

    if (fwrite(buffer, sizeof(char), size, fp) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

#endif

    retVal = ResultEnum::NormalEnd;

FINISH :

    if (fp != NULL)
    {
        fclose(fp);
    }

    if (buffer != NULL)
    {
        delete[] buffer;
        buffer = NULL;
    }

    return retVal;
}

/* 読み込み */
ResultEnum SettingManager::Load()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    FILE* fp = NULL;
    long index = 0;
    char buffer[40] = { 0 };
    char once[40] = { 0 };

    m_LastErrorNo = 0;

    if (m_ApAddress != NULL)
    {
        delete[] m_ApAddress;
        m_ApAddress = NULL;
    }

#ifdef FILETYPE_TEXT

    /* ファイルを開く */
    fp = fopen(&FILE_PATH[0], "r");
    if (fp == NULL)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* 読み込み */
    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_RobotSize.Length = atof(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_RobotSize.Width = atof(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_FarmSize.Length = atof(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_FarmSize.Width = atof(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_MapCount.X = atol(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_MapCount.Y = atol(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_WavePowerCoeff = atof(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_ApCount = atol(&once[0]);

    m_ApAddress = new RectStr[m_ApCount];
    for (index = 0; index < m_ApCount; index++)
    {
        fgets(&buffer[0], sizeof(buffer), fp);
        parseString(&buffer[0], &once[0], ',', sizeof(buffer));
        m_ApAddress[index].X = atol(&once[0]);

        fgets(&buffer[0], sizeof(buffer), fp);
        parseString(&buffer[0], &once[0], ',', sizeof(buffer));
        m_ApAddress[index].Y = atol(&once[0]);
    }

#else

    /* ファイルを開く */
    fp = fopen(&FILE_PATH[0], "rb");
    if (fp == NULL)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* 読み込み */
    if (fread(&m_RobotSize, sizeof(m_RobotSize), 1, fp) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    if (fread(&m_FarmSize, sizeof(m_FarmSize), 1, fp) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    if (fread(&m_MapCount, sizeof(m_MapCount), 1, fp) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    if (fread(&m_WavePowerCoeff, sizeof(m_WavePowerCoeff), 1, fp) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    if (fread(&m_ApCount, sizeof(m_ApCount), 1, fp) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    m_ApAddress = new RectStr[m_ApCount];
    if (fread(&m_ApAddress[0], sizeof(RectStr), m_ApCount, fp) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

#endif

    retVal = ResultEnum::NormalEnd;

FINISH:

    if (fp != NULL)
    {
        fclose(fp);
    }

    return retVal;
}

int SettingManager::GetLastErrorNo()
{
    return m_LastErrorNo;
}

/*--------------------*/
/* 設定値の取得・変更 */
/*--------------------*/
/* ロボットサイズ */
SizeStr SettingManager::GetRobotSize()
{
    return m_RobotSize;
}

void SettingManager::SetRobotSize(SizeStr* const size)
{
    m_RobotSize.Length = size->Length;
    m_RobotSize.Width = size->Width;
}

/* 畑のサイズ */
SizeStr SettingManager::GetFarmSize()
{
    return m_FarmSize;
}

void SettingManager::SetFarmSize(SizeStr* const size)
{
    m_FarmSize.Length = size->Length;
    m_FarmSize.Width = size->Width;
}

/* マップの個数 */
RectStr SettingManager::GetMapCount()
{
    return m_MapCount;
}

/* 電波の伝搬係数 */
float SettingManager::GetWavePowerCoeff()
{
    return m_WavePowerCoeff;
}

void SettingManager::SetWavePowerCoeff(const float coeff)
{
    m_WavePowerCoeff = coeff;
}

/* アクセスポイント */
long SettingManager::GetApCount()
{
    return m_ApCount;
}

void SettingManager::GetApAddress(RectStr* data)
{
    if (m_ApCount <= 0)
    {
        goto FINISH;
    }

    if (data == NULL)
    {
        data = new RectStr[m_ApCount];
    }

    memcpy(data, m_ApAddress, sizeof(RectStr) * m_ApCount);

FINISH :
    return;
}

void SettingManager::SetApInfo(const long count, RectStr* address)
{
    if (count <= 0)
    {
        goto FINISH;
    }

    if (m_ApAddress != NULL)
    {
        delete[] m_ApAddress;
        m_ApAddress = NULL;
    }

    m_ApAddress = new RectStr[count];
    memcpy(&m_ApAddress[0], address, sizeof(RectStr) * count);

FINISH :
    return;
}

/* 設定の合計サイズ取得 */
long SettingManager::calcurateSettingSize()
{
    long retVal = 0;

    retVal += sizeof(m_RobotSize);            /* ロボットサイズ */
    retVal += sizeof(m_FarmSize);             /* 畑のサイズ */
    retVal += sizeof(m_MapCount);             /* マップの個数 */
    retVal += sizeof(m_WavePowerCoeff);       /* 電波の伝搬係数 */
    retVal += sizeof(m_ApCount);              /* アクセスポイントの個数 */
    retVal += sizeof(RectStr) * m_ApCount;    /* アクセスポイントの座標 */

    return retVal;
}

void SettingManager::parseString(char* const src, char* const dest, const char delimiter, const long maxLen)
{
    long srcIndex = 0;
    long destIndex = 0;
    bool detected = false;

    for (srcIndex = 0; srcIndex < maxLen; srcIndex++)
    {
        if (src[srcIndex] == delimiter)
        {
            detected = true;
            continue;
        }

        if (detected == true)
        {
            dest[destIndex] = src[srcIndex];
            destIndex++;
        }

        if (src[srcIndex] == '\0')
        {
            break;
        }
    }
}

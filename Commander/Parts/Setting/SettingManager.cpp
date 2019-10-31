#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "SettingManager.h"

#define FILE_PATH   ("/home/pi/Information/Setting.txt")

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
    /* ロボットサイズ (横/縦/全高) : 600mm * 463mm * 418mm */
    m_RobotSize.Horizontal = 600.0;
    m_RobotSize.Vertical = 463.0;

    /* 畑の大きさ */
    m_FarmSize.Horizontal = 3000.0;
    m_FarmSize.Vertical = 3000.0;

    /* マップの個数 (= 自動計算) */
    UpdateMapCount();

    /*  電波の伝搬係数*/
    m_WavePowerCoeff = 2.0f;

    /* 草刈り開始時刻 */
    m_KusakariStart.DayOfWeek = DayOfWeekEnum::Nothing;
    m_KusakariStart.Hour = 0;
    m_KusakariStart.Minute = 0;

    /* 夜警開始時刻 */
    m_YakeiStart.DayOfWeek = DayOfWeekEnum::Nothing;
    m_YakeiStart.Hour = 0;
    m_YakeiStart.Minute = 0;

    /* 網羅完了判定割合 */
    m_MoveEndRate = 0.95f;

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

    m_LastErrorNo = 0;

    /* ファイルを開く */
    fp = fopen(&FILE_PATH[0], "w");
    if (fp == NULL)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* 書き込み */
    fprintf(fp, "RobotVertical,%lf\n", m_RobotSize.Vertical);
    fprintf(fp, "RobotHorizontal,%lf\n", m_RobotSize.Horizontal);
    fprintf(fp, "FarmVertical,%lf\n", m_FarmSize.Vertical);
    fprintf(fp, "FarmHorizontal,%lf\n", m_FarmSize.Horizontal);
    fprintf(fp, "WavePowerCoeff,%f\n", m_WavePowerCoeff);
    fprintf(fp, "KusakariDayOfWeek,%d\n", m_KusakariStart.DayOfWeek);
    fprintf(fp, "KusakariStartHour,%d\n", m_KusakariStart.Hour);
    fprintf(fp, "KusakariStartMinute,%d\n", m_KusakariStart.Minute);
    fprintf(fp, "YakeiDayOfWeek,%d\n", m_YakeiStart.DayOfWeek);
    fprintf(fp, "YakeiStartHour,%d\n", m_YakeiStart.Hour);
    fprintf(fp, "YakeiStartMinute,%d\n", m_YakeiStart.Minute);
    fprintf(fp, "MoveEndRate,%f\n", m_MoveEndRate);
    fprintf(fp, "APCount,%ld\n", m_ApCount);
    for (size = 0; size < m_ApCount; size++)
    {
        fprintf(fp, "AP%ld(X),%d\n", size + 1, m_ApAddress[size].X);
        fprintf(fp, "AP%ld(Y),%d\n", size + 1, m_ApAddress[size].Y);
    }

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
    m_RobotSize.Vertical = atof(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_RobotSize.Horizontal = atof(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_FarmSize.Vertical = atof(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_FarmSize.Horizontal = atof(&once[0]);

    /* マップの個数 (= 自動計算) */
    UpdateMapCount();

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_WavePowerCoeff = (float)atof(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_KusakariStart.DayOfWeek = (DayOfWeekEnum)atol(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_KusakariStart.Hour = (unsigned char)atol(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_KusakariStart.Minute = (unsigned char)atol(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_YakeiStart.DayOfWeek = (DayOfWeekEnum)atol(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_YakeiStart.Hour = (unsigned char)atol(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_YakeiStart.Minute = (unsigned char)atol(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_MoveEndRate = (float)atof(&once[0]);

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
void SettingManager::GetRobotSize(SizeStr* const size)
{
    size->Vertical = m_RobotSize.Vertical;
    size->Horizontal = m_RobotSize.Horizontal;
}

void SettingManager::SetRobotSize(SizeStr* const size)
{
    m_RobotSize.Vertical = size->Vertical;
    m_RobotSize.Horizontal = size->Horizontal;
}

/* 畑のサイズ */
void SettingManager::GetFarmSize(SizeStr* const size)
{
    size->Vertical = m_FarmSize.Vertical;
    size->Horizontal = m_FarmSize.Horizontal;
}

void SettingManager::SetFarmSize(SizeStr* const size)
{
    m_FarmSize.Vertical = size->Vertical;
    m_FarmSize.Horizontal = size->Horizontal;
}

/* マップの個数 */
void SettingManager::GetMapCount(RectStr* const count)
{
    count->X = m_MapCount.X;
    count->Y = m_MapCount.Y;
}

void SettingManager::UpdateMapCount()
{
    long xCount = (long)(m_FarmSize.Horizontal / m_RobotSize.Horizontal);
    long yCount = (long)(m_FarmSize.Vertical / m_RobotSize.Vertical);

    m_MapCount.X = (xCount * 2) + 2;
    m_MapCount.Y = (yCount * 2) + 2;
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

/* 草刈り開始時刻 */
SettingManager::TimeSettingStr SettingManager::GetKusakariStartTime()
{
    return m_KusakariStart;
}

void SettingManager::SetKusakariStartTime(TimeSettingStr* const date)
{
    m_KusakariStart.DayOfWeek = date->DayOfWeek;
    m_KusakariStart.Hour = date->Hour;
    m_KusakariStart.Minute = date->Minute;
}

/* 夜警開始時刻 */
SettingManager::TimeSettingStr SettingManager::GetYakeiStartTime()
{
    return m_YakeiStart;
}

void SettingManager::SetYakeStartTime(TimeSettingStr* const date)
{
    m_YakeiStart.DayOfWeek = date->DayOfWeek;
    m_YakeiStart.Hour = date->Hour;
    m_YakeiStart.Minute = date->Minute;
}

/* 網羅完了判定割合 */
float SettingManager::GetMoveEndRate()
{
    return m_MoveEndRate;
}

void SettingManager::SetMoveEndRate(const float rate)
{
    m_MoveEndRate = rate;
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

    retVal += sizeof(m_RobotSize);          /* ロボットサイズ */
    retVal += sizeof(m_FarmSize);           /* 畑のサイズ */
    retVal += sizeof(m_MapCount);           /* マップの個数 */
    retVal += sizeof(m_WavePowerCoeff);     /* 電波の伝搬係数 */
    retVal += sizeof(m_KusakariStart);      /* 草刈り開始時刻情報 */
    retVal += sizeof(m_YakeiStart);         /* 夜警開始時刻情報 */
    retVal += sizeof(m_ApCount);            /* アクセスポイントの個数 */
    retVal += sizeof(RectStr) * m_ApCount;  /* アクセスポイントの座標 */

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "Parts/ShareMemory/ShareMemory.h"
#include "PositionData.h"

/* 向いている方向をもとに、前進した場合の座標を算出するためのテーブル */
/* マップの [0][0] が北西方向としている */
const RectStr PositionData::m_ReferenceMap[CompassEnum::E_COMPASS_MAX]
= {
    /*   X       Y      */
    /*-------+----------*/
    {    0,     -1  },  /* 北 */
    {    1,     -1  },  /* 北東 */
    {    1,      0  },  /* 東 */
    {    1,      1  },  /* 南東 */
    {    0,      1  },  /* 南 */
    {   -1,      1  },  /* 南西 */
    {   -1,      0  },  /* 西 */
    {   -1,     -1  },  /* 北西 */
};

PositionData* const PositionData::m_This = new PositionData();

PositionData::PositionData()
 : m_Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::LogTypeEnum::BOTH_OUT)
 , m_LastErrorNo(ERROR_NOTHING)
{
    memset(&m_LogStr[0], 0x00, sizeof(m_LogStr));
}

PositionData::~PositionData()
{
    /* nop. */
}

PositionData* const PositionData::GetInstance()
{
    return m_This;
}

void PositionData::SetPosition(RectStr* const position)
{
    m_PrevPosition = m_Position;

    m_Position = *position;

    pShareMemory->Motor.PointX = position->X;
    pShareMemory->Motor.PointY = position->Y;

    /* 動作済みに変える */
    MoveMap* moveMap = MoveMap::GetInstance();
    moveMap->ChangeMoved(position);

    if ((m_PrevPosition.X != m_Position.X)
    ||  (m_PrevPosition.Y != m_Position.Y))
    {
        moveMap->Print();
    }
}

RectStr PositionData::GetPosition(void)
{
    return m_Position;
}

PositionData::CompassEnum PositionData::GetCompass()
{
    return m_Compass;
}

/* 向いている方角に進んだ時の座標を取得する */
void PositionData::GetNextPosition(RectStr* const position)
{
    position->X = m_Position.X + m_ReferenceMap[m_Compass].X;
    position->Y = m_Position.Y + m_ReferenceMap[m_Compass].Y;
}

/* 指定した方向に進んだ時の座標を取得する */
void PositionData::GetNextPosition(const DirectionEnum direction, RectStr* const position)
{
    CompassEnum nextCompass = convertDirectionToCompass(m_Compass, direction);
    position->X = m_Position.X + m_ReferenceMap[nextCompass].X;
    position->Y = m_Position.Y + m_ReferenceMap[nextCompass].Y;
}

/* 向いている方向を変える */
void PositionData::SetDirection(DirectionEnum const direction)
{
    m_Compass = convertDirectionToCompass(m_Compass, direction);
}

/*-----------------*/
/* 初期化用 処理群 */
/*-----------------*/
/* ファイルがあるか確認する */
bool PositionData::IsFileExist()
{
    bool retVal = false;
    FILE* fp = NULL;


    /* 読み取り属性で開けるか確認 */
    fp = fopen("/home/pi/Information/Position.dat", "r");
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
ResultEnum PositionData::Save()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    FILE* fp = NULL;

    m_LastErrorNo = ERROR_NOTHING;

    /* ファイルを開く */
    fp = fopen("/home/pi/Information/Position.dat", "w");
    if (fp == NULL)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* 書き込み */
    fprintf(fp, "CurrentX,%ld\n", m_Position.X);
    fprintf(fp, "CurrentY,%ld\n", m_Position.Y);
    fprintf(fp, "Compass,%d\n", m_Compass);


    retVal = ResultEnum::NormalEnd;

FINISH :

    if (fp != NULL)
    {
        fclose(fp);
    }

    return retVal;
}

/* 位置情報と向いている方角を読み込む */
ResultEnum PositionData::Load()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    FILE* fp = NULL;
    char buffer[40] = { 0 };
    char once[40] = { 0 };

    m_LastErrorNo = ERROR_NOTHING;

    /* ファイルを開く */
    fp = fopen("/home/pi/Information/Position.dat", "r");
    if (fp == NULL)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* 読み込み */
    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_Position.X = atol(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_Position.Y = atol(&once[0]);

    fgets(&buffer[0], sizeof(buffer), fp);
    parseString(&buffer[0], &once[0], ',', sizeof(buffer));
    m_Compass = (CompassEnum)atol(&once[0]);

    retVal = ResultEnum::NormalEnd;

FINISH:

    if (fp != NULL)
    {
        fclose(fp);
    }

    return retVal;
}

/* 初期データ生成 */
void PositionData::SetInitialData()
{
    m_Position.X = 1;
    m_Position.Y = 1;
    m_Compass = CompassEnum::E_COMPASS_EAST;
}

/* 最後に発生したシステムエラーを取得する */
int PositionData::GetLastError()
{
    return m_LastErrorNo;
}

/* 指定された方角に向きを加味した新たな方角を返す */
PositionData::CompassEnum PositionData::convertDirectionToCompass(const CompassEnum compass, const DirectionEnum direction)
{
    CompassEnum retVal = CompassEnum::E_COMPASS_MAX;
    long tempCompass = (long)compass;

    tempCompass += (long)direction;
    tempCompass %= (long)CompassEnum::E_COMPASS_MAX;
    retVal = (CompassEnum)tempCompass;

    return retVal;
}

/* 文字列を特定の文字で分割する */
void PositionData::parseString(char* const src, char* const dest, const char delimiter, const long maxLen)
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

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

/* 向いている方向から先のマップデータを全て変更する */
void PositionData::FillAreaMap(const unsigned char value)
{
    AreaMap* areaMap = AreaMap::GetInstance();
    SettingManager* setting = SettingManager::GetInstance();

    RectStr mapCount;
    setting->GetMapCount(&mapCount);

    RectStr target = m_Position;
    while (1)
    {
        target.X += m_ReferenceMap[m_Compass].X;
        target.Y += m_ReferenceMap[m_Compass].Y;

        if ((target.X <= 0) || (mapCount.X <= target.X)
        ||  (target.Y <= 0) || (mapCount.Y <= target.Y))
        {
            break;
        }

        areaMap->Set(&target, value);
    }
}

/* 現在の位置でマップを二分して、右・左 どちらの方が未移動エリアが多いか判断する */
DirectionEnum PositionData::JudgeNotMoveArea()
{
    DirectionEnum retVal = DirectionEnum::E_DIR_RIGHT;

    RectStr startPos1;
    RectStr startPos2;
    RectStr endPos1;
    RectStr endPos2;
    RectStr mapSize;
    long x = 0;
    long y = 0;
    unsigned long area1NotMovedCount = 0;
    unsigned long area2NotMovedCount = 0;
    
    SettingManager* setting = SettingManager::GetInstance();
    AreaMap* areaMap = AreaMap::GetInstance();
    MoveMap* moveMap = MoveMap::GetInstance();

    setting->GetMapCount(&mapSize);

    if ((m_Compass == CompassEnum::E_COMPASS_NORTH) || (m_Compass == CompassEnum::E_COMPASS_SOUTH))
    {
        startPos1.X = 1;
        startPos1.Y = 1;
        endPos1.X = m_Position.X - 1;
        endPos1.Y = mapSize.Y - 1;

        startPos2.X = m_Position.X + 1;
        startPos2.Y = 1;
        endPos2.X = mapSize.X - 1;
        endPos2.Y = mapSize.Y - 1;
    }
    else if ((m_Compass == CompassEnum::E_COMPASS_WEST) || (m_Compass == CompassEnum::E_COMPASS_EAST))
    {
        startPos1.X = 1;
        startPos1.Y = 1;
        endPos1.X = mapSize.X - 1;
        endPos1.Y = m_Position.Y - 1;
        
        startPos2.X = 1;
        startPos2.Y = m_Position.Y + 1;
        endPos2.X = mapSize.X - 1;
        endPos2.Y = mapSize.Y - 1;
    }
    else
    {
        /* @todo : ロボットが斜めを向いている場合はとりあえず右側で返す */
        goto FINISH;
    }

    for (y = startPos1.Y; y < endPos1.Y; y++)
    {
        for (x = startPos1.X; x < endPos1.X; x++)
        {
            if (areaMap->IsMovable(x, y) == false)
            {
                continue;
            }

            if (areaMap->Get(x, y) == AreaMap::OBSTACLE_VALUE)
            {
                continue;
            }

            if (moveMap->IsNotMove(x, y) == true)
            {
                area1NotMovedCount++;
            }
        }
    }

    for (y = startPos2.Y; y < endPos2.Y; y++)
    {
        for (x = startPos2.X; x < endPos2.X; x++)
        {
            if (areaMap->IsMovable(x, y) == false)
            {
                continue;
            }

            if (areaMap->Get(x, y) == AreaMap::OBSTACLE_VALUE)
            {
                continue;
            }

            if (moveMap->IsNotMove(x, y) == true)
            {
                area2NotMovedCount++;
            }
        }
    }

    /* エリア 2 の方が多い */
    if (area1NotMovedCount < area2NotMovedCount)
    {
        if (m_Compass == CompassEnum::E_COMPASS_NORTH)
        {
            retVal = DirectionEnum::E_DIR_RIGHT;
        }
        else if (m_Compass == CompassEnum::E_COMPASS_SOUTH)
        {
            retVal = DirectionEnum::E_DIR_LEFT;
        }
        else if (m_Compass == CompassEnum::E_COMPASS_WEST)
        {
            retVal = DirectionEnum::E_DIR_LEFT;
        }
        else
        {
            retVal = DirectionEnum::E_DIR_RIGHT;
        }
    }
    else
    {
        if (m_Compass == CompassEnum::E_COMPASS_NORTH)
        {
            retVal = DirectionEnum::E_DIR_LEFT;
        }
        else if (m_Compass == CompassEnum::E_COMPASS_SOUTH)
        {
            retVal = DirectionEnum::E_DIR_RIGHT;
        }
        else if (m_Compass == CompassEnum::E_COMPASS_WEST)
        {
            retVal = DirectionEnum::E_DIR_RIGHT;
        }
        else
        {
            retVal = DirectionEnum::E_DIR_LEFT;
        }
    }


FINISH :
    return retVal;
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

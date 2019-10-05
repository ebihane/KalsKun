#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "Parts/ShareMemory/ShareMemory.h"
#include "PositionData.h"

/* �����Ă�����������ƂɁA�O�i�����ꍇ�̍��W���Z�o���邽�߂̃e�[�u�� */
/* �}�b�v�� [0][0] ���k�������Ƃ��Ă��� */
const RectStr PositionData::m_ReferenceMap[CompassEnum::E_COMPASS_MAX]
= {
    /*   X       Y      */
    /*-------+----------*/
    {    0,     -1  },  /* �k */
    {    1,     -1  },  /* �k�� */
    {    1,      0  },  /* �� */
    {    1,      1  },  /* �쓌 */
    {    0,      1  },  /* �� */
    {   -1,      1  },  /* �쐼 */
    {   -1,      0  },  /* �� */
    {   -1,     -1  },  /* �k�� */
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

    /* ����ς݂ɕς��� */
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

/* �����Ă�����p�ɐi�񂾎��̍��W���擾���� */
void PositionData::GetNextPosition(RectStr* const position)
{
    position->X = m_Position.X + m_ReferenceMap[m_Compass].X;
    position->Y = m_Position.Y + m_ReferenceMap[m_Compass].Y;
}

/* �w�肵�������ɐi�񂾎��̍��W���擾���� */
void PositionData::GetNextPosition(const DirectionEnum direction, RectStr* const position)
{
    CompassEnum nextCompass = convertDirectionToCompass(m_Compass, direction);
    position->X = m_Position.X + m_ReferenceMap[nextCompass].X;
    position->Y = m_Position.Y + m_ReferenceMap[nextCompass].Y;
}

/* �����Ă��������ς��� */
void PositionData::SetDirection(DirectionEnum const direction)
{
    m_Compass = convertDirectionToCompass(m_Compass, direction);
}

/* �����Ă�����������̃}�b�v�f�[�^��S�ĕύX���� */
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

/* ���݂̈ʒu�Ń}�b�v��񕪂��āA�E�E�� �ǂ���̕������ړ��G���A�����������f���� */
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
        /* @todo : ���{�b�g���΂߂������Ă���ꍇ�͂Ƃ肠�����E���ŕԂ� */
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

    /* �G���A 2 �̕������� */
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
/* �������p �����Q */
/*-----------------*/
/* �t�@�C�������邩�m�F���� */
bool PositionData::IsFileExist()
{
    bool retVal = false;
    FILE* fp = NULL;


    /* �ǂݎ�葮���ŊJ���邩�m�F */
    fp = fopen("/home/pi/Information/Position.dat", "r");
    if (fp == NULL)
    {
        /* �J���Ȃ��ꍇ�͑��݂��Ȃ����̂Ƃ��� */
        goto FINISH;
    }

    fclose(fp);

    retVal = true;

FINISH:
    return retVal;
}

/* �ۑ� */
ResultEnum PositionData::Save()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    FILE* fp = NULL;

    m_LastErrorNo = ERROR_NOTHING;

    /* �t�@�C�����J�� */
    fp = fopen("/home/pi/Information/Position.dat", "w");
    if (fp == NULL)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* �������� */
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

/* �ʒu���ƌ����Ă�����p��ǂݍ��� */
ResultEnum PositionData::Load()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    FILE* fp = NULL;
    char buffer[40] = { 0 };
    char once[40] = { 0 };

    m_LastErrorNo = ERROR_NOTHING;

    /* �t�@�C�����J�� */
    fp = fopen("/home/pi/Information/Position.dat", "r");
    if (fp == NULL)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* �ǂݍ��� */
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

/* �����f�[�^���� */
void PositionData::SetInitialData()
{
    m_Position.X = 1;
    m_Position.Y = 1;
    m_Compass = CompassEnum::E_COMPASS_EAST;
}

/* �Ō�ɔ��������V�X�e���G���[���擾���� */
int PositionData::GetLastError()
{
    return m_LastErrorNo;
}

/* �w�肳�ꂽ���p�Ɍ��������������V���ȕ��p��Ԃ� */
PositionData::CompassEnum PositionData::convertDirectionToCompass(const CompassEnum compass, const DirectionEnum direction)
{
    CompassEnum retVal = CompassEnum::E_COMPASS_MAX;
    long tempCompass = (long)compass;

    tempCompass += (long)direction;
    tempCompass %= (long)CompassEnum::E_COMPASS_MAX;
    retVal = (CompassEnum)tempCompass;

    return retVal;
}

/* ����������̕����ŕ������� */
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

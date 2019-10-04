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

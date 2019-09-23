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

/* �w�肵�����W�̃f�[�^���擾���� (-1 �̏ꍇ�A�͈͊O) */
unsigned char MappingData::Get(RectStr* position)
{
    return Get(position->X, position->Y);
}

unsigned char MappingData::Get(const long x, const long y)
{
    unsigned char retVal = 0;

    /* ���W���͈͊O */
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

/* �w�肵�����W���� ([1, 1]�𒆐S�Ƃ���8����) �̃f�[�^���擾���� (-1 �͔͈͊O) */
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

/* �w�肵�����W�̃f�[�^��ύX���� */
ResultEnum MappingData::Set(RectStr* const position, const unsigned char value)
{
    return Set(position->X, position->Y, value);
}

ResultEnum MappingData::Set(const long x, const long y, const unsigned char value)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    /* ���W���͈͊O */
    if (isInRange(x, y) != true)
    {
        goto FINISH;
    }

    /* �͈͊O�f�[�^�̓Z�b�g�s�� */
    if (value == OUT_OF_AREA_VALUE)
    {
        goto FINISH;
    }

    m_MapData[y][x] = value;

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

/* �Ō�ɔ��������V�X�e���G���[���擾���� */
int MappingData::GetLastError()
{
    return m_LastErrorNo;
}

/*-----------------*/
/* �������p �����Q */
/*-----------------*/
/* �t�@�C�������邩�m�F���� */
bool MappingData::IsFileExist()
{
    bool retVal = false;
    FILE* fp = NULL;

    
    /* �ǂݎ�葮���ŊJ���邩�m�F */
    fp = fopen(&m_FilePath[0], "rb");
    if (fp == NULL)
    {
        /* �J���Ȃ��ꍇ�͑��݂��Ȃ����̂Ƃ��� */
        goto FINISH;
    }

    fclose(fp);

    retVal = true;

FINISH :
    return retVal;
}

/* Mapping Data ���m�ۂ��� */
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

/* Mapping Data ��������� */
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

/* Mapping Data �̗̈�T�C�Y�ƌ��݂̒l���t�@�C���ɕۑ����� */
ResultEnum MappingData::Save()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    FILE* fp = NULL;
    SettingManager* setting = SettingManager::GetInstance();
    RectStr mapCount = { 0 };
    setting->GetMapCount(&mapCount);

    /* MapData �����݂��Ȃ� */
    if (m_MapData == NULL)
    {
        goto FINISH;
    }

    m_LastErrorNo = ERROR_NOTHING;

    /* �t�@�C���I�[�v�� */
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
            /* �}�b�v�f�[�^ �������� */
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

/* Mapping Data �̗̈�T�C�Y�ƕۑ����̒l���t�@�C������擾���AMappingData �𕜌����� */
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

    /* �t�@�C���I�[�v�� */
    fp = fopen(&m_FilePath[0], "rb");
    if (fp == NULL)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* �̈�m�� */
    if (Allocate() != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    for (long y = 0; y < mapCount.Y; y++)
    {
        for (long x = 0; x < mapCount.X; x++)
        {
            /* �}�b�v�f�[�^ �ǂݍ��� */
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

/* �w�肳�ꂽ���W���}�b�v�f�[�^�̗̈�����m�F���� */
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

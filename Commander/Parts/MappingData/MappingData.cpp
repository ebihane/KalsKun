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

/* �w�肵�����W�̃f�[�^���擾���� (-1 �̏ꍇ�A�͈͊O) */
unsigned char MappingData::Get(const unsigned long x, const unsigned long y)
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

/* �w�肵�����W�̃f�[�^��ύX���� */
ResultEnum MappingData::Set(const unsigned long x, const unsigned long y, const unsigned char value)
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
/* Mapping Data ���m�ۂ��� */
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

/* Mapping Data ��������� */
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

/* Mapping Data �̗̈�T�C�Y�ƌ��݂̒l���t�@�C���ɕۑ����� */
ResultEnum MappingData::Save()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    int fd = -1;

    /* MapData �����݂��Ȃ� */
    if (m_MapData == NULL)
    {
        goto FINISH;
    }

    m_LastErrorNo = 0;

    /* �t�@�C���I�[�v�� */
    fd = open(&m_FilePath[0], O_RDWR | O_CREAT, 0666);
    if (fd < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* �� �������� */
    if (write(fd, &m_Width, sizeof(m_Width)) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* ���� �������� */
    if (write(fd, &m_Height, sizeof(m_Height)) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* �}�b�v�f�[�^ �������� */
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

/* Mapping Data �̗̈�T�C�Y�ƕۑ����̒l���t�@�C������擾���AMappingData �𕜌����� */
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

    /* �t�@�C���I�[�v�� */
    fd = open(&m_FilePath[0], O_RDONLY);
    if (fd < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* �� �ǂݍ��� */
    if (read(fd, &width, sizeof(width)) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* ���� �ǂݍ��� */
    if (read(fd, &height, sizeof(height)) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    /* �̈�m�� */
    if (Allocate(width, height) != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    /* �}�b�v�f�[�^ �ǂݍ��� */
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

/* �w�肳�ꂽ���W���}�b�v�f�[�^�̗̈�����m�F���� */
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

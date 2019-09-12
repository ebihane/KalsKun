#pragma once

#include "Include/Common.h"
#include "Parts/ShareMemory/ShareMemory.h"

class MappingData
{
public:

    static const int            ERROR_NOTHING = 0;
    static const unsigned char  OUT_OF_AREA_VALUE = 0xFF;

    MappingData(char* const path);
    virtual ~MappingData();

    /* �w�肵�����W�̃f�[�^���擾���� (-1 �̏ꍇ�A�͈͊O) */
    unsigned char Get(const unsigned long x, const unsigned long y);

    /* �w�肵�����W���� ([1, 1]�𒆐S�Ƃ���8����) �̃f�[�^���擾���� */
    void Get(const unsigned long x, const unsigned long y, unsigned char** const value);

    /* �w�肵�����W�̃f�[�^��ύX���� */
    ResultEnum Set(const unsigned long x, const unsigned long y, const unsigned char value);

    /* �Ō�ɔ��������V�X�e���G���[���擾���� */
    int GetLastError();

    /*-----------------*/
    /* �������p �����Q */
    /*-----------------*/
    /* Mapping Data ���m�ۂ��� */
    ResultEnum Allocate(const unsigned long width, const unsigned long height);

    /* Mapping Data ��������� */
    void Free();

    /* Mapping Data �̗̈�T�C�Y�ƌ��݂̒l���t�@�C���ɕۑ����� */
    ResultEnum Save();

    /* Mapping Data �̗̈�T�C�Y�ƕۑ����̒l���t�@�C������擾���AMappingData �𕜌����� */
    ResultEnum Load();



protected :

private :

    static const unsigned char  PointXArray[3][3];
    static const unsigned char  PointYArray[3][3];

    char            m_FilePath[80]; /* �t�@�C���p�X */

    unsigned long   m_Width;        /* �� (= X ��) */
    unsigned long   m_Height;       /* ���� (= Y ��) */
    unsigned char** m_MapData;      /* �}�b�v�f�[�^ */

    int             m_LastErrorNo;  /* �Ō�ɔ��������V�X�e���G���[�ԍ� */

    /* �w�肳�ꂽ���W���}�b�v�f�[�^�̗̈�����m�F���� */
    bool isInRange(const unsigned long x, const unsigned long y);

};
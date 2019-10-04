#pragma once

#include "Include/Common.h"

class MappingData
{
public:

    static const int            ERROR_NOTHING = 0;
    static const unsigned char  OUT_OF_AREA_VALUE = 0xFF;

    MappingData(char* const path);
    virtual ~MappingData();

    /* �w�肵�����W�̃f�[�^���擾���� (-1 �̏ꍇ�A�͈͊O) */
    unsigned char Get(RectStr* position);
    unsigned char Get(const long x, const long y);

    /* �w�肵�����W���� ([1, 1]�𒆐S�Ƃ���8����) �̃f�[�^���擾���� */
    void Get(RectStr* position, unsigned char** const value);
    void Get(const long x, const long y, unsigned char** const value);

    /* �w�肵�����W�̃f�[�^��ύX���� */
    ResultEnum Set(RectStr* const position, const unsigned char value);
    ResultEnum Set(const long x, const long y, const unsigned char value);

    /* �Ō�ɔ��������V�X�e���G���[���擾���� */
    int GetLastError();

    /* �}�b�v�̏�Ԃ�\������ */
    void Print();

    /*-----------------*/
    /* �������p �����Q */
    /*-----------------*/
    /* �t�@�C�������邩�m�F���� */
    bool IsFileExist();

    /* Mapping Data ���m�ۂ��� */
    ResultEnum Allocate();

    /* Mapping Data ��������� */
    void Free();

    /* Mapping Data �̗̈�T�C�Y�ƌ��݂̒l���t�@�C���ɕۑ����� */
    ResultEnum Save();

    /* Mapping Data �̗̈�T�C�Y�ƕۑ����̒l���t�@�C������擾���AMappingData �𕜌����� */
    ResultEnum Load();

    /* �����f�[�^���� */
    virtual void SetInitialData() = 0;

protected :

    /* �w�肳�ꂽ���W���}�b�v�f�[�^�̗̈�����m�F���� */
    bool isInRange(const long x, const long y);

private :

    char            m_FilePath[80]; /* �t�@�C���p�X */
    unsigned char** m_MapData;      /* �}�b�v�f�[�^ */
    int             m_LastErrorNo;  /* �Ō�ɔ��������V�X�e���G���[�ԍ� */

};
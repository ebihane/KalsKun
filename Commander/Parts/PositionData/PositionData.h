#pragma once

#include "Include/Common.h"
#include "Logger/Logger.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Parts/Setting/SettingManager.h"
#include "Parts/MappingData/AreaMap.h"
#include "Parts/MappingData/MoveMap.h"

/* �����ƕ��p���Ǘ����邽�߂̃V���O���g���N���X */
/* ��ʑw�ł́u�O�㍶�E�v�Ŏw�����o���������A���{�b�g�̈ʒu�͐�ΓI�ȃf�[�^ (= ���p) �Ŕ��f���������߁A���̃N���X�����܂��� */
/*  �����F�u�O�㍶�E�v����Ƃ����A���{�b�g�̌����̏��B���{�b�g�̎p���ɂ���ĎQ�Ƃ�����p���ς�� */
/*  ���p�F�u������k�v����Ƃ����A���{�b�g�������Ă�����p�̏��B*/
class PositionData
{
public :

    static const int    ERROR_NOTHING = 0;

    /* ���p�萔 */
    typedef enum
    {
        E_COMPASS_NORTH = 0,    /* 0: �k */
        E_COMPASS_NORTHEAST,    /* 1: �k�� */
        E_COMPASS_EAST,         /* 2: �� */
        E_COMPASS_SOUTHEAST,    /* 3: �쓌 */
        E_COMPASS_SOUTH,        /* 4: �� */
        E_COMPASS_SOUTHWEST,    /* 5: �쐼 */
        E_COMPASS_WEST,         /* 6: �� */
        E_COMPASS_NORTHWEST,    /* 7: �k�� */
        E_COMPASS_MAX,          /* 8: ���p�� */
    } CompassEnum;

    /* �C���X�^���X���擾���� */
    static PositionData* const GetInstance();

    /* ���݂̍��W���Z�b�g���� */
    void SetPosition(RectStr* const position);

    /* ���݂̍��W���擾���� */
    RectStr GetPosition(void);

    /* ���݌����Ă�����p���擾���� */
    CompassEnum GetCompass();

    /* �����Ă�����p�ɐi�񂾎��̍��W���擾���� */
    void GetNextPosition(RectStr* const position);

    /* �w�肵�������ɐi�񂾎��̍��W���擾���� */
    void GetNextPosition(const DirectionEnum direction, RectStr* const position);

    /* �����Ă��������ς��� */
    void SetDirection(DirectionEnum const direction);

    /*-----------------*/
    /* �������p �����Q */
    /*-----------------*/
    /* �t�@�C�������邩�m�F���� */
    bool IsFileExist();

    /* �ʒu���ƌ����Ă�����p���t�@�C���ɕۑ����� */
    ResultEnum Save();

    /* �ʒu���ƌ����Ă�����p��ǂݍ��� */
    ResultEnum Load();

    /* �����f�[�^���� */
    void SetInitialData();

    /* �Ō�ɔ��������V�X�e���G���[���擾���� */
    int GetLastError();

protected :

private :

    static const RectStr m_ReferenceMap[CompassEnum::E_COMPASS_MAX];    /* ���W�ړ������Z�o�}�b�v */

    static PositionData* const m_This;  /* ���N���X�C���X�^���X */

    Logger      m_Logger;               /* Logger */
    RectStr     m_Position;             /* ���݈ʒu */
    RectStr     m_PrevPosition;         /* ���O�̈ʒu */
    CompassEnum m_Compass;              /* ���݌����Ă�����p */
    int         m_LastErrorNo;          /* �Ō�ɔ��������V�X�e���G���[�ԍ� */
    char        m_LogStr[LOG_OUT_MAX];  /* ���O�o�̓o�b�t�@ */

    PositionData();
    virtual ~PositionData();

    /* �w�肳�ꂽ���p�Ɍ��������������V���ȕ��p��Ԃ� */
    CompassEnum convertDirectionToCompass(const CompassEnum compass, const DirectionEnum direction);

    /* ����������̕����ŕ������� */
    void parseString(char* const src, char* const dest, const char delimiter, const long maxLen);

};

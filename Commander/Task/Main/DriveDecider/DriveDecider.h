#pragma once

#include "Logger/Logger.h"
#include "Measure/Stopwatch.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Parts/Setting/SettingManager.h"
#include "Parts/MappingData/AreaMap.h"
#include "Parts/MappingData/MoveMap.h"
#include "Parts/PositionData/PositionData.h"

class DriveDecider
{
public :

    DriveDecider();
    virtual ~DriveDecider();

    /* ���쌈�� (�ʏ펞) */
    virtual MotorCommandEnum Decide();

    /* ���쌈�� (�ʍs�֎~�G���A���m��) */
    virtual MotorCommandEnum DecideForRoadClosed();

    /* ���쌈�� (��Q�����m��) */
    virtual MotorCommandEnum DecideForAvoidance();

    /* �ԗ��������� */
    virtual bool IsComplete();

    /* �^�[���񐔎擾 */
    unsigned long GetTurnCount();

protected :

    Logger m_Logger;            /* Log Accessor */
    AreaMap* m_AreaMap;         /* �G���A�}�b�v�N���X�C���X�^���X */
    MoveMap* m_MoveMap;         /* ����}�b�v�N���X�C���X�^���X */
    PositionData* m_Position;   /* �ʒu���Ǘ��N���X�C���X�^���X  */
    SettingManager* m_Setting;  /* �ݒ�Ǘ��N���X�C���X�^���X */
    Stopwatch m_FrontKeepWatch; /* �^�[�����O�i�L�[�v���Ԍv���p�X�g�b�v�E�H�b�` */
    MotorCommandEnum m_Preview; /* ���O�̌�����e */
    RectStr m_MapCount;         /* ���̃T�C�Y */

    unsigned long m_TurnCount;  /* �^�[���� */

private :

};

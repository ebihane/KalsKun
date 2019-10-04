#pragma once

#include "Parts/Setting/SettingManager.h"
#include "Parts/MappingData/AreaMap.h"
#include "Parts/MappingData/MoveMap.h"
#include "Parts/PositionData/PositionData.h"
#include "Task/Main/DriveDecider/DriveDecider.h"
#include "Task/Main/Sequencer/SequencerBase.h"

class YakeiKun : public SequencerBase
{
public:

    YakeiKun();
    virtual ~YakeiKun();

protected:


private:

    /* ��ԏ��\���� */
    typedef struct
    {
        MoveTypeEnum        MoveType;       /* from �O���J���� : ����w�� */
        DetectTypeEnum      Human;          /* from �����J���� : �l���m��� */
        DetectTypeEnum      Animal;         /* from �����J���� : �������m��� */
        DetectTypeEnum      Redwave;        /* from ���ӃJ���� : �ԊO�����m��� */
        MotorCommandEnum    CurrentMove;    /* from ���[�^ : ���݂̓����� */
        ControlModeEnum     ControlMode;    /* from ���[�^ : �R���g���[�����[�h */
    } StateInfoStr;

    /* ����w���\���� */
    typedef struct
    {
        MelodyModeEnum      Melody;         /* to �u�U�[ : �u�U�[�o�͎w�� */
        LightModeEnum       Light;          /* to ���C�g : ���C�g�o�͎w�� */
        MotorCommandEnum    MotorCommand;   /* to ���[�^ : ����w�� */
        CutterDriveEnum     Cutter;         /* to ���[�^ : ������n��]�w�� */
    } DriveInfoStr;

    StateInfoStr        m_PreviewState;     /* �O������̏�ԏ�� */
    DriveInfoStr        m_PreviewDrive;     /* �O������̓���w�� */
    DriveDecider        m_DriveDecider;     /* �쓮���쌈��N���X�C���X�^���X */

    ResultEnum initializeCore();
    void destroyCore();
    SequenceTypeEnum processCore();

    /* ��Ԏ��W */
    void correctCurrentState(StateInfoStr* const state);

    /* �o�͌��� */
    MelodyModeEnum decideMelodyMode(StateInfoStr* const state);
    LightModeEnum decideLightMode(StateInfoStr* const state);
    MotorCommandEnum decideMotorCommand(StateInfoStr* const state);
    CutterDriveEnum decideCutterDrive(StateInfoStr* const state);

    /* �w�� */
    void sendMotorMessage(DriveInfoStr* const drive);

    /* ���񓮍�̌��� */
    SequenceTypeEnum decideNextSequence(StateInfoStr* const state);

};

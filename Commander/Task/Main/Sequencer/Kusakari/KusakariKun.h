#pragma once

#include "Parts/Setting/SettingManager.h"
#include "Parts/MappingData/AreaMap.h"
#include "Parts/MappingData/MoveMap.h"
#include "Task/Main/Sequencer/SequencerBase.h"

class KusakariKun : public SequencerBase
{
public:

    KusakariKun();
    virtual ~KusakariKun();

protected:


private:

    /* ��ԏ��\���� */
    typedef struct
    {
        MoveTypeEnum        MoveType;       /* from �O���J���� : ����w�� */
        DetectTypeEnum      Animal;         /* from �����J���� : �������m��� */
        SizeStr             RealPosition;   /* from ���[�^ : ���{�b�g�����݈ʒu */
        RectStr             Position;       /* from ���[�^ : ���{�b�g�ʒu���W */
        MotorCommandEnum    CurrentMove;    /* from ���[�^ : ���݂̓����� */
        ControlModeEnum     ControlMode;    /* from ���[�^ : �R���g���[�����[�h */
        long                TurnCount;      /* from �i�ߓ� : �^�[���� */
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

    SettingManager*     m_Setting;          /* �ݒ�Ǘ��N���X�C���X�^���X */
    AreaMap*            m_AreaMap;          /* �G���A�}�b�v�N���X�C���X�^���X */
    MoveMap*            m_MoveMap;          /* ����}�b�v�N���X�C���X�^���X */

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

    /* ���[�e�B���e�B */
    RectStr convertRealPointToMapPoint(SizeStr* const pRealPoint);
};

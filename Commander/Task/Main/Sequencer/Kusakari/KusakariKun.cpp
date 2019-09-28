#include <stdio.h>
#include "Logger/Logger.h"
#include "Parts/CommanderCommon.h"
#include "KusakariKun.h"

KusakariKun::KusakariKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_KUSAKARI)
 , m_Setting(NULL)
 , m_AreaMap(NULL)
 , m_MoveMap(NULL)
{
    /* nop. */
}

KusakariKun::~KusakariKun()
{
    /* nop. */
}

ResultEnum KusakariKun::initializeCore()
{
    /* �N���X�C���X�^���X�擾 */
    m_Setting = SettingManager::GetInstance();
    m_AreaMap = AreaMap::GetInstance();
    m_MoveMap = MoveMap::GetInstance();

    /* ���O�̏�ԏ��������� */
    m_PreviewState.MoveType = MoveTypeEnum::NOT_REQUEST;
    m_PreviewState.Animal = DetectTypeEnum::NOT_DETECT;
    m_PreviewState.Position.X = -1;
    m_PreviewState.Position.Y = -1;
    m_PreviewState.CurrentMove = MotorCommandEnum::E_COMMAND_MAX;
    m_PreviewState.ControlMode = ControlModeEnum::E_MODE_MANUAL;

    /* ���O�̓������������ */
    m_PreviewDrive.Melody = MelodyModeEnum::E_MELODY_SILENT;
    m_PreviewDrive.Light = LightModeEnum::E_LIGHT_OFF;
    m_PreviewDrive.MotorCommand = MotorCommandEnum::E_COMMAND_STOP;
    m_PreviewDrive.Cutter = CutterDriveEnum::E_CUTTER_STOP;

    return ResultEnum::NormalEnd;
}

void KusakariKun::destroyCore()
{
    pShareMemory->Commander.LightMode = LightModeEnum::E_LIGHT_OFF;
    pShareMemory->Commander.MelodyMode = MelodyModeEnum::E_MELODY_SILENT;
}

SequencerBase::SequenceTypeEnum KusakariKun::processCore()
{
    SequenceTypeEnum retVal = SequenceTypeEnum::E_SEQ_ERROR;
    StateInfoStr state;
    DriveInfoStr drive;


    /* �����蒆�ł��邱�Ƃ������o�� */
    digitalWrite(IO_KUSATORI_MODE, HIGH);
    digitalWrite(IO_YAKEI_MODE, LOW);

    /* ��Ԏ��W */
    correctCurrentState(&state);
    
    /* ��Ԃ��瓮������� */
    drive.Melody = decideMelodyMode(&state);
    drive.Light = decideLightMode(&state);
    drive.MotorCommand = decideMotorCommand(&state);
    drive.Cutter = decideCutterDrive(&state);

    /* �o�� */
    pShareMemory->Commander.MelodyMode = drive.Melody;
    pShareMemory->Commander.LightMode = drive.Light;
    sendMotorMessage(&drive);

    /* ����}�b�v�X�V */
    m_MoveMap->ChangeMoved(&state.Position);

    /* ���̓�������� */
    retVal = decideNextSequence(&state);

    /* ����̔������ێ� */
    m_PreviewState = state;
    m_PreviewDrive = drive;

#if 0
    if ((m_PrevRect.X != point.X)
    ||  (m_PrevRect.Y != point.Y))
    {
        char log[64] = { 0 };
        snprintf(&log[0], sizeof(log), "[processCore] Move. Array[%ld, %ld] Real[%f, %f]\n", point.X, point.Y);
        m_Logger.LOG_INFO(log);
    }

    m_PrevRect.X = point.X;
    m_PrevRect.Y = point.Y;
#endif

    return retVal;
}

void KusakariKun::correctCurrentState(StateInfoStr* const state)
{
    SizeStr realPoint;
    realPoint.Width = (double)pShareMemory->Motor.PointX;
    realPoint.Length = (double)pShareMemory->Motor.PointY;

    state->MoveType = pShareMemory->FrontCamera.MoveType;
    state->Animal = pShareMemory->AnimalCamera.Animal;
    state->Position = convertRealPointToMapPoint(&realPoint);
    state->CurrentMove = pShareMemory->Motor.Command;
    state->ControlMode = pShareMemory->Motor.RemoteMode;
}

MelodyModeEnum KusakariKun::decideMelodyMode(StateInfoStr* const state)
{
    MelodyModeEnum retVal = MelodyModeEnum::E_MELODY_SILENT;

    if (state->Animal != DetectTypeEnum::NOT_DETECT)
    {
        retVal = MelodyModeEnum::E_MELODY_WARNING;
    }
    else
    {
        retVal = MelodyModeEnum::E_MELODY_SOUND_1;
    }

    return retVal;
}

LightModeEnum KusakariKun::decideLightMode(StateInfoStr* const state)
{
    LightModeEnum retVal = LightModeEnum::E_LIGHT_OFF;

    if (state->Animal != DetectTypeEnum::NOT_DETECT)
    {
        retVal = LightModeEnum::E_LIGHT_BLINK;
    }
    else
    {
        retVal = LightModeEnum::E_LIGHT_OFF;
    }

    return retVal;
}

MotorCommandEnum KusakariKun::decideMotorCommand(StateInfoStr* const state)
{
    MotorCommandEnum retVal = MotorCommandEnum::E_COMMAND_STOP;

    /* @todo : �}�b�v����i�s���������߂镔���͖����� */
    /* ����ł͐ԃe�[�v���m�܂őO�i�A��Q������Ή�� */

    if (state->MoveType == MoveTypeEnum::TURN)
    {
        retVal = MotorCommandEnum::E_COMMAND_R_TURN;
    }
    else if (state->MoveType == MoveTypeEnum::AVOIDANCE)
    {
        retVal = MotorCommandEnum::E_COMMAND_AVOID;
    }
    else
    {
        retVal = MotorCommandEnum::E_COMMAND_FRONT;
    }

    return retVal;
}

CutterDriveEnum KusakariKun::decideCutterDrive(StateInfoStr* const state)
{
    /* �����胂�[�h���͕K����] */
    return CutterDriveEnum::E_CUTTER_DRIVE;
}

void KusakariKun::sendMotorMessage(DriveInfoStr* const drive)
{
    EventInfo ev = { 0 };

    /* �O��ƕς��Ȃ��ꍇ�͑��M�s�v */
    if ((drive->MotorCommand == m_PreviewDrive.MotorCommand)
    &&  (drive->Cutter == m_PreviewDrive.Cutter))
    {
        goto FINISH;
    }

    ev.Code = 2;
    ev.lParam[0] = drive->MotorCommand;
    ev.lParam[1] = drive->Cutter;
    m_SendQueue.Send((char*)"MotorComm", &ev, sizeof(ev));


FINISH:
    return;
}

SequencerBase::SequenceTypeEnum KusakariKun::decideNextSequence(StateInfoStr* const state)
{
    SequenceTypeEnum retVal = MY_SEQUENCE_TYPE;

    /* �S�ԗ������̏ꍇ�� IDLE */
    if (m_MoveMap->IsComplete() == true)
    {
        m_Logger.LOG_INFO("[decideNextSequence] Kusakari Finish!!!\n");
        retVal = SequenceTypeEnum::E_SEQ_IDLE;
        goto FINISH;
    }

    /* Manual ���[�h�ؑ֎��� IDLE */
    if (state->ControlMode == ControlModeEnum::E_MODE_MANUAL)
    {
        m_Logger.LOG_INFO("[decideNextSequence] Manual Mode.\n");
        retVal = SequenceTypeEnum::E_SEQ_IDLE;
        goto FINISH;
    }

FINISH :
    return retVal;
}

RectStr KusakariKun::convertRealPointToMapPoint(SizeStr* const pRealPoint)
{
    RectStr retVal = { 0 };
    SettingManager* setting = SettingManager::GetInstance();

    /* ���{�b�g�̑傫�����擾 */
    SizeStr robotSize = { 0 };
    setting->GetRobotSize(&robotSize);

    /* 1 �}�X���̑傫���́A���{�b�g�T�C�Y�̔��� */
    robotSize.Length /= 2;
    robotSize.Width /= 2;

    /* ���W�̓��{�b�g�T�C�Y�Ŋ������l */
    retVal.X = (long)(pRealPoint->Width / robotSize.Width);
    retVal.Y = (long)(pRealPoint->Length / robotSize.Length);

    /* ���������ӂɂ킴�Ɛi���֎~�G���A��݂��Ă��邽�߁A+1 */
    retVal.X += 1;
    retVal.Y += 1;

    return retVal;
}

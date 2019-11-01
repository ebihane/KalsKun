#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "Logger/Logger.h"
#include "Parts/CommanderCommon.h"
#include "Parts/MappingData/MoveMap.h"
#include "KusakariKun.h"

KusakariKun::KusakariKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_KUSAKARI)
{
    /* nop. */
}

KusakariKun::~KusakariKun()
{
    /* nop. */
}

ResultEnum KusakariKun::initializeCore()
{
    /* ���[�^�}�C�R���� Auto ���[�h�ɐ؂�ւ��� */
    EventInfo ev = { 0 };
    ev.Code = 3;
    ev.lParam[0] = (long)ControlModeEnum::E_MODE_AUTO;
    m_SendQueue.Send((char*)"MotorComm", &ev, sizeof(ev));

    char cnt = 0;
    while (1)
    {
        if (pShareMemory->Motor.RemoteMode == ControlModeEnum::E_MODE_AUTO)
        {
            m_Logger.LOG_INFO("[KusakariKun] AUTO Change.\n");
            break;
        }

        cnt++;
        if (50 <= cnt)
        {
            m_Logger.LOG_ERROR("[KusakariKun] AUTO Change Timeout.\n");
            break;
        }

        delay(100);
    }

    /* ���O�̏�ԏ��������� */
    m_PreviewState.MoveType = MoveTypeEnum::NOT_REQUEST;
    m_PreviewState.Animal = DetectTypeEnum::NOT_DETECT;
    m_PreviewState.CurrentMove = MotorCommandEnum::E_COMMAND_MAX;
    m_PreviewState.ControlMode = ControlModeEnum::E_MODE_MANUAL;

    /* ���O�̓������������ */
    m_PreviewDrive.Melody = MelodyModeEnum::E_MELODY_SILENT;
    m_PreviewDrive.Light = LightModeEnum::E_LIGHT_OFF;
    m_PreviewDrive.MotorCommand = MotorCommandEnum::E_COMMAND_STOP;
    m_PreviewDrive.Cutter = CutterDriveEnum::E_CUTTER_STOP;

    /* �J�n�����L�^ */
    struct timeval tmVal;
    gettimeofday(&tmVal, 0);
    struct tm* tmPtr = localtime(&tmVal.tv_sec);
    pShareMemory->Commander.LastStartDateTime.Year = tmPtr->tm_year + 1900;
    pShareMemory->Commander.LastStartDateTime.Month = tmPtr->tm_mon + 1;
    pShareMemory->Commander.LastStartDateTime.Day = tmPtr->tm_mday;
    pShareMemory->Commander.LastStartDateTime.Hour = tmPtr->tm_hour;
    pShareMemory->Commander.LastStartDateTime.Minute = tmPtr->tm_min;
    pShareMemory->Commander.LastStartDateTime.Second = tmPtr->tm_sec;

    return ResultEnum::NormalEnd;
}

void KusakariKun::destroyCore()
{
    pShareMemory->Commander.LightMode = LightModeEnum::E_LIGHT_OFF;
    pShareMemory->Commander.MelodyMode = MelodyModeEnum::E_MELODY_SILENT;

    /* �ʒu���ۑ� */
    PositionData* position = PositionData::GetInstance();
    position->Save();

    /* ����}�b�v���ۑ� */
    MoveMap* moveMap = MoveMap::GetInstance();
    moveMap->Save();

    /* ���[�^�}�C�R���� Manual ���[�h�ɐ؂�ւ��� */
    EventInfo ev = { 0 };
    ev.Code = 3;
    ev.lParam[0] = (long)ControlModeEnum::E_MODE_MANUAL;
    m_SendQueue.Send((char*)"MotorComm", &ev, sizeof(ev));

    char cnt = 0;
    while (1)
    {
        if (pShareMemory->Motor.RemoteMode == ControlModeEnum::E_MODE_MANUAL)
        {
            m_Logger.LOG_INFO("[KusakariKun] MANUAL Change.\n");
            break;
        }

        cnt++;
        if (50 <= cnt)
        {
            m_Logger.LOG_ERROR("[KusakariKun] MANUAL Change Timeout.\n");
            break;
        }

        delay(100);
    }
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

    /* ���̓�������� */
    retVal = decideNextSequence(&state);

    /* ����̔������ێ� */
    m_PreviewState = state;
    m_PreviewDrive = drive;

    return retVal;
}

void KusakariKun::correctCurrentState(StateInfoStr* const state)
{
    state->MoveType = pShareMemory->FrontCamera.MoveType;
    state->Animal = pShareMemory->AnimalCamera.Animal;
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
        retVal = MelodyModeEnum::E_MELODY_SOUND_2;
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

    /* �ԃe�[�v���m */
    if (state->MoveType == MoveTypeEnum::TURN)
    {
        retVal = m_DriveDecider.DecideForRoadClosed();
    }
    /* ��Q�����m */
    else if (state->MoveType == MoveTypeEnum::AVOIDANCE)
    {
        retVal = m_DriveDecider.DecideForAvoidance();
    }
    /* �����Ȃ� */
    else
    {
        retVal = m_DriveDecider.Decide();
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
    MoveMap* moveMap = MoveMap::GetInstance();

#ifdef AUTO_CHANGE_TRIGGER
    /* Manual ���[�h�ؑ֎��� IDLE */
    if (state->ControlMode == ControlModeEnum::E_MODE_MANUAL)
    {
        m_Logger.LOG_INFO("[decideNextSequence] Manual Mode.\n");
        retVal = SequenceTypeEnum::E_SEQ_IDLE;
        goto FINISH;
    }
#endif

#ifdef TAPE_COUNT_EXECUTE
    /* �^�[���� 6 ��ȏ�� IDLE */
    if (6 <= m_DriveDecider.GetTurnCount())
    {
        m_Logger.LOG_INFO("[decideNextSequence] Trun Count Arrival.\n");
        retVal = SequenceTypeEnum::E_SEQ_IDLE;
        goto FINISH;
    }
#else
    /* �S�ԗ������̏ꍇ�� IDLE */
    if (m_DriveDecider.IsComplete() == true)
    {
        m_Logger.LOG_INFO("[decideNextSequence] Kusakari Finish!!!\n");
        moveMap->UpdateMovedValue();
        retVal = SequenceTypeEnum::E_SEQ_IDLE;
        goto FINISH;
    }
#endif

FINISH :
    return retVal;
}

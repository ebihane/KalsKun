#include <time.h>
#include <sys/time.h>
#include "Parts/CommanderCommon.h"
#include "YakeiKun.h"

YakeiKun::YakeiKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_YAKEI)
{
    /* nop. */
}

YakeiKun::~YakeiKun()
{
    /* nop. */
}

ResultEnum YakeiKun::initializeCore()
{
    /* ���O�̏�ԏ��������� */
    m_PreviewState.MoveType = MoveTypeEnum::NOT_REQUEST;
    m_PreviewState.Human = DetectTypeEnum::NOT_DETECT;
    m_PreviewState.Animal = DetectTypeEnum::NOT_DETECT;
    m_PreviewState.Redwave = DetectTypeEnum::NOT_DETECT;
    m_PreviewState.CurrentMove = MotorCommandEnum::E_COMMAND_MAX;
    m_PreviewState.ControlMode = ControlModeEnum::E_MODE_MANUAL;

    /* ���O�̓������������ */
    m_PreviewDrive.Melody = MelodyModeEnum::E_MELODY_SILENT;
    m_PreviewDrive.Light = LightModeEnum::E_LIGHT_OFF;
    m_PreviewDrive.MotorCommand = MotorCommandEnum::E_COMMAND_STOP;
    m_PreviewDrive.Cutter = CutterDriveEnum::E_CUTTER_DRIVE;

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

void YakeiKun::destroyCore()
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
}

SequencerBase::SequenceTypeEnum YakeiKun::processCore()
{
    SequenceTypeEnum retVal = SequenceTypeEnum::E_SEQ_ERROR;
    StateInfoStr state;
    DriveInfoStr drive;

    /* ��x���ł��邱�Ƃ������o�� */
    digitalWrite(IO_KUSATORI_MODE, LOW);
    digitalWrite(IO_YAKEI_MODE, HIGH);

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

void YakeiKun::correctCurrentState(StateInfoStr* const state)
{
    state->MoveType = pShareMemory->FrontCamera.MoveType;
    state->Human = pShareMemory->AnimalCamera.Human;
    state->Animal = pShareMemory->AnimalCamera.Animal;
    state->Redwave = pShareMemory->AroundCamera.Detect;
    state->CurrentMove = pShareMemory->Motor.Command;
    state->ControlMode = pShareMemory->Motor.RemoteMode;
}

MelodyModeEnum YakeiKun::decideMelodyMode(StateInfoStr* const state)
{
    MelodyModeEnum retVal = MelodyModeEnum::E_MELODY_SILENT;

    if ((state->Human != DetectTypeEnum::NOT_DETECT)
    ||  (state->Animal != DetectTypeEnum::NOT_DETECT)
    ||  (state->Redwave != DetectTypeEnum::NOT_DETECT))
    {
        retVal = MelodyModeEnum::E_MELODY_WARNING;
    }
    else
    {
        retVal = MelodyModeEnum::E_MELODY_SILENT;
    }

    return retVal;
}

LightModeEnum YakeiKun::decideLightMode(StateInfoStr* const state)
{
    LightModeEnum retVal = LightModeEnum::E_LIGHT_OFF;

    if ((state->Human != DetectTypeEnum::NOT_DETECT)
    ||  (state->Animal != DetectTypeEnum::NOT_DETECT)
    ||  (state->Redwave != DetectTypeEnum::NOT_DETECT))
    {
        retVal = LightModeEnum::E_LIGHT_BLINK;
    }
    else
    {
        retVal = LightModeEnum::E_LIGHT_ON;
    }

    return retVal;
}

MotorCommandEnum YakeiKun::decideMotorCommand(StateInfoStr* const state)
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

CutterDriveEnum YakeiKun::decideCutterDrive(StateInfoStr* const state)
{
    /* ��x���[�h���͕K����~ */
    return CutterDriveEnum::E_CUTTER_STOP;
}

void YakeiKun::sendMotorMessage(DriveInfoStr* const drive)
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

SequencerBase::SequenceTypeEnum YakeiKun::decideNextSequence(StateInfoStr* const state)
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
        m_Logger.LOG_INFO("[decideNextSequence] Yakei Finish!!!\n");
        moveMap->UpdateMovedValue();
        retVal = SequenceTypeEnum::E_SEQ_IDLE;
        goto FINISH;
    }

#endif

FINISH:
    return retVal;
}

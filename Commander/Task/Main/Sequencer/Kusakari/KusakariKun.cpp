#include <stdio.h>
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
    /* 直前の状態情報を初期化 */
    m_PreviewState.MoveType = MoveTypeEnum::NOT_REQUEST;
    m_PreviewState.Animal = DetectTypeEnum::NOT_DETECT;
    m_PreviewState.CurrentMove = MotorCommandEnum::E_COMMAND_MAX;
    m_PreviewState.ControlMode = ControlModeEnum::E_MODE_MANUAL;

    /* 直前の動作情報を初期化 */
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

    /* 位置情報保存 */
    PositionData* position = PositionData::GetInstance();
    position->Save();

    /* 動作マップ情報保存 */
    MoveMap* moveMap = MoveMap::GetInstance();
    moveMap->Save();
}

SequencerBase::SequenceTypeEnum KusakariKun::processCore()
{
    SequenceTypeEnum retVal = SequenceTypeEnum::E_SEQ_ERROR;
    StateInfoStr state;
    DriveInfoStr drive;


    /* 草刈り中であることを示す出力 */
    digitalWrite(IO_KUSATORI_MODE, HIGH);
    digitalWrite(IO_YAKEI_MODE, LOW);

    /* 状態収集 */
    correctCurrentState(&state);

    /* 状態から動作を決定 */
    drive.Melody = decideMelodyMode(&state);
    drive.Light = decideLightMode(&state);
    drive.MotorCommand = decideMotorCommand(&state);
    drive.Cutter = decideCutterDrive(&state);

    /* 出力 */
    pShareMemory->Commander.MelodyMode = drive.Melody;
    pShareMemory->Commander.LightMode = drive.Light;
    sendMotorMessage(&drive);

    /* 次の動作を決定 */
    retVal = decideNextSequence(&state);

    /* 今回の判定情報を保持 */
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

    /* 赤テープ検知 */
    if (state->MoveType == MoveTypeEnum::TURN)
    {
        retVal = m_DriveDecider.DecideForRoadClosed();
    }
    /* 障害物検知 */
    else if (state->MoveType == MoveTypeEnum::AVOIDANCE)
    {
        retVal = m_DriveDecider.DecideForAvoidance();
    }
    /* 何もなし */
    else
    {
        retVal = m_DriveDecider.Decide();
    }

    return retVal;
}

CutterDriveEnum KusakariKun::decideCutterDrive(StateInfoStr* const state)
{
    /* 草刈りモード時は必ず回転 */
    return CutterDriveEnum::E_CUTTER_DRIVE;
}

void KusakariKun::sendMotorMessage(DriveInfoStr* const drive)
{
    EventInfo ev = { 0 };

    /* 前回と変わらない場合は送信不要 */
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

    /* Manual モード切替時は IDLE */
    if (state->ControlMode == ControlModeEnum::E_MODE_MANUAL)
    {
        m_Logger.LOG_INFO("[decideNextSequence] Manual Mode.\n");
        retVal = SequenceTypeEnum::E_SEQ_IDLE;
        goto FINISH;
    }

#ifdef TAPE_COUNT_EXECUTE
    /* ターン回数 6 回以上で IDLE */
    if (6 <= m_DriveDecider.GetTurnCount())
    {
        m_Logger.LOG_INFO("[decideNextSequence] Trun Count Arrival.\n");
        retVal = SequenceTypeEnum::E_SEQ_IDLE;
        goto FINISH;
    }
#else
    /* 全網羅完了の場合は IDLE */
    if (moveMap->IsComplete() == true)
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

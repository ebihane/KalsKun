#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include "Parts/CommanderCommon.h"
#include "HimajinKun.h"

HimajinKun::HimajinKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_IDLE)
 , m_SettingManager(NULL)
{
    m_SettingManager = SettingManager::GetInstance();
}

HimajinKun::~HimajinKun()
{
    /* nop. */
}

ResultEnum HimajinKun::initializeCore()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    EventInfo ev = { 0 };

    ev.Code = 2;
    ev.lParam[0] = MotorCommandEnum::E_COMMAND_STOP;
    ev.lParam[1] = CutterDriveEnum::E_CUTTER_STOP;

    if (m_SendQueue.Send((char*)"MotorComm", &ev, sizeof(EventInfo)) != ResultEnum::NormalEnd)
    {
        char log[40] = { 0 };
        snprintf(&log[0], sizeof(log), "[initializeCore] Queue Send failed. errno[%d]\n", m_SendQueue.GetLastError());
        m_Logger.LOG_ERROR(log);
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;

FINISH:
    return retVal;
}

void HimajinKun::destroyCore()
{
    /* nop. */
}

SequencerBase::SequenceTypeEnum HimajinKun::processCore()
{
    SequenceTypeEnum retVal = E_SEQ_IDLE;
    SettingManager::TimeSettingStr current;
    SettingManager::TimeSettingStr kusakari = m_SettingManager->GetKusakariStartTime();
    SettingManager::TimeSettingStr yakei = m_SettingManager->GetYakeiStartTime();
    struct timeval tmVal;
    struct tm* tmPtr = NULL;

    /* Idle 中であることを示す出力 */
    digitalWrite(IO_KUSATORI_MODE, HIGH);
    digitalWrite(IO_YAKEI_MODE, HIGH);

    /* 音無し・ライト OFF */
    pShareMemory->Commander.MelodyMode = MelodyModeEnum::E_MELODY_SILENT;
    pShareMemory->Commander.LightMode = LightModeEnum::E_LIGHT_BLINK;

    /* モータマイコンで Auto モードに切り替わった */
    if (pShareMemory->Motor.RemoteMode == E_MODE_AUTO)
    {
        /* 草刈りモードに移行 */
        retVal = SequenceTypeEnum::E_SEQ_KUSAKARI;
        goto FINISH;
    }

    /* 現在時刻を取得 */
    gettimeofday(&tmVal, 0);
    tmPtr = localtime(&tmVal.tv_sec);

    /* 判定用構造体にセット */
    current.DayOfWeek = (SettingManager::DayOfWeekEnum)tmPtr->tm_wday;
    current.Hour = (unsigned char)tmPtr->tm_hour;
    current.Minute = (unsigned char)tmPtr->tm_min;

    /* 草刈り指定時刻になった */
    if (isTimeArrival(&current, &kusakari) == true)
    {
        /* 草刈りモードに移行 */
        m_Logger.LOG_INFO("[processCore] Kusakari Start!!\n");
        retVal = SequenceTypeEnum::E_SEQ_KUSAKARI;
        goto FINISH;
    }

    /* 夜警指定時刻になった */
    if (isTimeArrival(&current, &yakei) == true)
    {
        /* 夜警モードに移行 */
        m_Logger.LOG_INFO("[processCore] Yakei Start!!\n");
        retVal = SequenceTypeEnum::E_SEQ_YAKEI;
        goto FINISH;
    }

    retVal = MY_SEQUENCE_TYPE;

FINISH :
    return retVal;
}

bool HimajinKun::isTimeArrival(SettingManager::TimeSettingStr* current, SettingManager::TimeSettingStr* target)
{
    bool retVal = false;

    /* 動作無し */
    if (target->DayOfWeek == SettingManager::DayOfWeekEnum::Nothing)
    {
        goto FINISH;
    }

    /* 毎日ではない場合 */
    if (target->DayOfWeek != SettingManager::DayOfWeekEnum::Everyday)
    {
        /* 曜日不一致 */
        if (current->DayOfWeek != target->DayOfWeek)
        {
            goto FINISH;
        }
    }

    /* 時 不一致 */
    if (current->Hour != target->Hour)
    {
        goto FINISH;
    }

    /* 分 不一致 */
    if (current->Minute != target->Minute)
    {
        goto FINISH;
    }

    retVal = true;


FINISH :
    return retVal;
}

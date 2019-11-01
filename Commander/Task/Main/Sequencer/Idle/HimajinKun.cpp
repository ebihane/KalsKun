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

#ifdef AUTO_CHANGE_TRIGGER
    /* モータマイコンで Auto モードに切り替わった */
    if (pShareMemory->Motor.RemoteMode == E_MODE_AUTO)
    {
        /* 草刈りモードに移行 */
        m_Logger.LOG_INFO("[processCore] Kusakari Start!! (From Remote Controller)\n");
        retVal = SequenceTypeEnum::E_SEQ_KUSAKARI;
        goto FINISH;
    }
#endif

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
#if 0
        /* モータマイコンを Auto モードに切り替える */
        EventInfo ev = { 0 };
        ev.Code = 3;
        ev.lParam[0] = (long)ControlModeEnum::E_MODE_AUTO;
        m_SendQueue.Send((char*)"MotorComm", &ev, sizeof(ev));
#endif
        /* 草刈りモードに移行 */
        m_Logger.LOG_INFO("[processCore] Kusakari Start!! (DateTime Arrivaled)\n");
        retVal = SequenceTypeEnum::E_SEQ_KUSAKARI;
        goto FINISH;
    }

    /* 夜警指定時刻になった */
    if (isTimeArrival(&current, &yakei) == true)
    {
#if 0
        /* モータマイコンを Auto モードに切り替える */
        EventInfo ev = { 0 };
        ev.Code = 3;
        ev.lParam[0] = (long)ControlModeEnum::E_MODE_AUTO;
        m_SendQueue.Send((char*)"MotorComm", &ev, sizeof(ev));
#endif
        /* 夜警モードに移行 */
        m_Logger.LOG_INFO("[processCore] Yakei Start!! (DateTime Arrivaled)\n");
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
    struct timeval tmVal;
    struct tm* tmPtr = NULL;
    long year = 0;
    long month = 0;
    long day = 0;
    long hour = 0;
    long minute = 0;

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

    /* 直近の開始時刻と一緒の場合は開始しない */
    gettimeofday(&tmVal, 0);
    tmPtr = localtime(&tmVal.tv_sec);
    year = tmPtr->tm_year + 1900;
    month = tmPtr->tm_mon + 1;
    day = tmPtr->tm_mday;
    hour = tmPtr->tm_hour;
    minute = tmPtr->tm_min;

    if ((year == pShareMemory->Commander.LastStartDateTime.Year)
    &&  (month == pShareMemory->Commander.LastStartDateTime.Month)
    &&  (day == pShareMemory->Commander.LastStartDateTime.Day)
    &&  (hour == pShareMemory->Commander.LastStartDateTime.Hour)
    &&  (minute == pShareMemory->Commander.LastStartDateTime.Minute))
    {
        goto FINISH;
    }

    retVal = true;


FINISH :
    return retVal;
}

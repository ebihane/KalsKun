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

    /* Idle ���ł��邱�Ƃ������o�� */
    digitalWrite(IO_KUSATORI_MODE, HIGH);
    digitalWrite(IO_YAKEI_MODE, HIGH);

    /* �������E���C�g OFF */
    pShareMemory->Commander.MelodyMode = MelodyModeEnum::E_MELODY_SILENT;
    pShareMemory->Commander.LightMode = LightModeEnum::E_LIGHT_BLINK;

#ifdef AUTO_CHANGE_TRIGGER
    /* ���[�^�}�C�R���� Auto ���[�h�ɐ؂�ւ���� */
    if (pShareMemory->Motor.RemoteMode == E_MODE_AUTO)
    {
        /* �����胂�[�h�Ɉڍs */
        m_Logger.LOG_INFO("[processCore] Kusakari Start!! (From Remote Controller)\n");
        retVal = SequenceTypeEnum::E_SEQ_KUSAKARI;
        goto FINISH;
    }
#endif

    /* ���ݎ������擾 */
    gettimeofday(&tmVal, 0);
    tmPtr = localtime(&tmVal.tv_sec);

    /* ����p�\���̂ɃZ�b�g */
    current.DayOfWeek = (SettingManager::DayOfWeekEnum)tmPtr->tm_wday;
    current.Hour = (unsigned char)tmPtr->tm_hour;
    current.Minute = (unsigned char)tmPtr->tm_min;

    /* ������w�莞���ɂȂ��� */
    if (isTimeArrival(&current, &kusakari) == true)
    {
#if 0
        /* ���[�^�}�C�R���� Auto ���[�h�ɐ؂�ւ��� */
        EventInfo ev = { 0 };
        ev.Code = 3;
        ev.lParam[0] = (long)ControlModeEnum::E_MODE_AUTO;
        m_SendQueue.Send((char*)"MotorComm", &ev, sizeof(ev));
#endif
        /* �����胂�[�h�Ɉڍs */
        m_Logger.LOG_INFO("[processCore] Kusakari Start!! (DateTime Arrivaled)\n");
        retVal = SequenceTypeEnum::E_SEQ_KUSAKARI;
        goto FINISH;
    }

    /* ��x�w�莞���ɂȂ��� */
    if (isTimeArrival(&current, &yakei) == true)
    {
#if 0
        /* ���[�^�}�C�R���� Auto ���[�h�ɐ؂�ւ��� */
        EventInfo ev = { 0 };
        ev.Code = 3;
        ev.lParam[0] = (long)ControlModeEnum::E_MODE_AUTO;
        m_SendQueue.Send((char*)"MotorComm", &ev, sizeof(ev));
#endif
        /* ��x���[�h�Ɉڍs */
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

    /* ���얳�� */
    if (target->DayOfWeek == SettingManager::DayOfWeekEnum::Nothing)
    {
        goto FINISH;
    }

    /* �����ł͂Ȃ��ꍇ */
    if (target->DayOfWeek != SettingManager::DayOfWeekEnum::Everyday)
    {
        /* �j���s��v */
        if (current->DayOfWeek != target->DayOfWeek)
        {
            goto FINISH;
        }
    }

    /* �� �s��v */
    if (current->Hour != target->Hour)
    {
        goto FINISH;
    }

    /* �� �s��v */
    if (current->Minute != target->Minute)
    {
        goto FINISH;
    }

    /* ���߂̊J�n�����ƈꏏ�̏ꍇ�͊J�n���Ȃ� */
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

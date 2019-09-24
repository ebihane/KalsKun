#define IO_TEST

#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include "Parts/CommanderCommon.h"
#include "HimajinKun.h"

HimajinKun::HimajinKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_IDLE)
 , m_LastRemoteType(ControlModeEnum::E_MODE_MANUAL)
{
    /* nop. */
}

HimajinKun::~HimajinKun()
{
    /* nop. */
}

ResultEnum HimajinKun::initializeCore()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    EventInfo ev = { 0 };

    m_Buzzer = new BuzzerController();
    if (m_Buzzer == NULL)
    {
        goto FINISH;
    }
    m_Buzzer->Run();

#ifdef IO_TEST
    m_Light = new LightController();
    if (m_Light == NULL)
    {
        goto FINISH;
    }
    m_Light->Run();
#endif

    m_LastRemoteType = pShareMemory->Motor.RemoteMode;

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
#ifdef IO_TEST
    if (m_Light != NULL)
    {
        m_Light->Stop(10);
        delete m_Light;
        m_Light = NULL;
    }
#endif

    if (m_Buzzer != NULL)
    {
        m_Buzzer->Stop(10);
        delete m_Buzzer;
        m_Buzzer = NULL;
    }
}

SequencerBase::SequenceTypeEnum HimajinKun::processCore()
{
    SequenceTypeEnum retVal = E_SEQ_IDLE;

    /* Idle 中であることを示す出力 */
    digitalWrite(IO_KUSATORI_MODE, HIGH);
    digitalWrite(IO_YAKEI_MODE, HIGH);

    pShareMemory->Commander.MelodyMode = MelodyModeEnum::E_MELODY_SOUND_1;
#ifdef IO_TEST
    pShareMemory->Commander.LightMode = LightModeEnum::E_LIGHT_BLINK;
#endif

#ifdef LAST_TEST_PROC

    /* モータマイコンで Auto モードに切り替わった */
    if ((m_LastRemoteType == E_MODE_MANUAL) && (pShareMemory->Motor.RemoteMode == E_MODE_AUTO))
    {
        /* 草刈りモードに移行 */
        retVal = SequenceTypeEnum::E_SEQ_KUSAKARI;
        goto FINISH;
    }

    m_LastRemoteType = pShareMemory->Motor.RemoteMode;

#else
    struct timeval tmVal;
    struct tm* tmPtr = NULL;

    /* 現在時刻を取得 */
    gettimeofday(&tmVal, 0);
    tmPtr = localtime(&tmVal.tv_sec);

    /* 指定した時刻になった */
#endif


FINISH :
    return retVal;
}

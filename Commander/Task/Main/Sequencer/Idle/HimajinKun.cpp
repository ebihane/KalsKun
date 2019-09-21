#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include "Parts/CommanderCommon.h"
#include "HimajinKun.h"

HimajinKun::HimajinKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_IDLE)
 , m_LastRemoteType(ControlModeEnum::E_MODE_MANUAL)
{

}

HimajinKun::~HimajinKun()
{

}

ResultEnum HimajinKun::initializeCore()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    EventInfo ev = { 0 };

    m_LastRemoteType = pShareMemory->MotorState.RemoteMode;

    ev.Code = 2;
    ev.lParam[0] = MotorCommandEnum::E_COMMAND_FRONT;
    ev.lParam[1] = CutterDriveEnum::E_CUTTER_DRIVE;

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

}

SequencerBase::SequenceTypeEnum HimajinKun::processCore()
{
    SequenceTypeEnum retVal = E_SEQ_IDLE;

#ifdef LAST_TEST_PROC

    /* モータマイコンで Auto モードに切り替わった */
    if ((m_LastRemoteType == E_MODE_MANUAL) && (pShareMemory->MotorState.RemoteMode == E_MODE_AUTO))
    {
        /* 草刈りモードに移行 */
        retVal = SequenceTypeEnum::E_SEQ_KUSAKARI;
        goto FINISH;
    }

    m_LastRemoteType = pShareMemory->MotorState.RemoteMode;

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

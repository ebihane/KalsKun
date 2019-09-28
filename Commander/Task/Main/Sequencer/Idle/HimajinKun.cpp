#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include "Parts/CommanderCommon.h"
#include "HimajinKun.h"

HimajinKun::HimajinKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_IDLE)
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

    /* Idle ���ł��邱�Ƃ������o�� */
    digitalWrite(IO_KUSATORI_MODE, HIGH);
    digitalWrite(IO_YAKEI_MODE, HIGH);

    pShareMemory->Commander.MelodyMode = MelodyModeEnum::E_MELODY_SILENT;
    pShareMemory->Commander.LightMode = LightModeEnum::E_LIGHT_OFF;

#ifdef LAST_TEST_PROC

    /* ���[�^�}�C�R���� Auto ���[�h�ɐ؂�ւ���� */
    if (pShareMemory->Motor.RemoteMode == E_MODE_AUTO)
    {
        /* �����胂�[�h�Ɉڍs */
        retVal = SequenceTypeEnum::E_SEQ_KUSAKARI;
        goto FINISH;
    }

#else
    struct timeval tmVal;
    struct tm* tmPtr = NULL;

    /* ���ݎ������擾 */
    gettimeofday(&tmVal, 0);
    tmPtr = localtime(&tmVal.tv_sec);

    /* �w�肵�������ɂȂ��� */
#endif


FINISH :
    return retVal;
}

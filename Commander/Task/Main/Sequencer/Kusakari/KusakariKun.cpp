#include <stdio.h>
#include "Logger/Logger.h"
#include "Parts/CommanderCommon.h"
#include "Parts/MappingData/AreaMap.h"
#include "Parts/MappingData/MoveMap.h"
#include "KusakariKun.h"

KusakariKun::KusakariKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_KUSAKARI)
 , m_TapeDetect(0)
 , m_PrevAvoidance(0)
{

}

KusakariKun::~KusakariKun()
{

}

ResultEnum KusakariKun::initializeCore()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    EventInfo ev = { 0 };

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

    m_PrevAvoidance = 0;

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

void KusakariKun::destroyCore()
{
    /* nop. */
}

SequencerBase::SequenceTypeEnum KusakariKun::processCore()
{
    SequenceTypeEnum retVal = SequenceTypeEnum::E_SEQ_ERROR;
    bool sendExist = false;
    long avoidance = pShareMemory->FrontCamera.Avoidance;
    EventInfo ev = { 0 };
    MoveMap* moveMap = MoveMap::GetInstance();

    /* �����蒆�ł��邱�Ƃ������o�� */
    digitalWrite(IO_KUSATORI_MODE, HIGH);
    digitalWrite(IO_YAKEI_MODE, LOW);

    /* ����}�b�v�X�V */
    long pointX = pShareMemory->MotorState.PointX;
    long pointY = pShareMemory->MotorState.PointY;
    moveMap->ChangeMoved(pointX, pointY);

    /* �S�ԗ������̏ꍇ�� IDLE */
    if (moveMap->IsComplete() == true)
    {
        m_Logger.LOG_INFO("[processCore] Kusakari Finish!!!\n");
        retVal = SequenceTypeEnum::E_SEQ_IDLE;
        goto FINISH;
    }

    /* �O���J��������̉���w�� */
    if (avoidance != m_PrevAvoidance)
    {
        /* �����Ȃ� */
        if (pShareMemory->FrontCamera.Avoidance == 0)
        {
            sendExist = true;
            ev.Code = 0;
            ev.lParam[0] = (long)MotorCommandEnum::E_COMMAND_FRONT;
        }
        /* ��Q�������m */
        else if (pShareMemory->FrontCamera.Avoidance == 1)
        {
            sendExist = true;
            ev.Code = 0;
            ev.lParam[0] = (long)MotorCommandEnum::E_COMMAND_AVOID;
        }
        /* �ԃe�[�v�����m */
        else if (pShareMemory->FrontCamera.Avoidance == 2)
        {
            if (m_TapeDetect == 0)
            {
                /* 1 ��ڂ� U �^�[�� */
                sendExist = true;
                ev.Code = 0;
                ev.lParam[0] = (long)MotorCommandEnum::E_COMMAND_U_TURN;
            }
            else if (pShareMemory->MotorState.Command == MotorCommandEnum::E_COMMAND_AVOID)
            {
                /* ��𒆂̏ꍇ�͉������Ȃ� */
                sendExist = false;
            }
            else
            {
                /* 2 ��ڂ� IDLE */
                m_Logger.LOG_INFO("[processCore] Red Tape 2nd Detected!!!\n");
                retVal = SequenceTypeEnum::E_SEQ_IDLE;
                goto FINISH;
            }
        }
        else
        {
            /* nop. */
            sendExist = false;
        }

        if (sendExist == true)
        {
            m_SendQueue.Send((char*)"MotorComm", &ev, sizeof(ev));
        }
    }

    m_PrevRect.X = pointX;
    m_PrevRect.Y = pointY;
    m_PrevAvoidance = avoidance;

    retVal = MY_SEQUENCE_TYPE;

FINISH :
    return retVal;
}

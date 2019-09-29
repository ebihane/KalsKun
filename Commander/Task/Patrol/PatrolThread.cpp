#include <stdio.h>
#include <string.h>
#include "Parts/ShareMemory/ShareMemory.h"
#include "PatrolThread.h"

using namespace std;

PatrolThread::PatrolThread()
 : LoopThreadBase((char*)"PatrolThread", 100, TypeEnum::TIMER_STOP)
{
    /* nop. */
}

PatrolThread::~PatrolThread()
{
    /* nop. */
}

void PatrolThread::AddTarget(ThreadBase* const target)
{
    if (target != NULL)
    {
        ThreadInfo* info = new ThreadInfo();
        target->GetName(&(info->Name[0]));
        info->Thread = target;
        info->PreviewState = false;
        m_TargetList.push_back(info);
    }
}

ResultEnum PatrolThread::initializeCore()
{
    strncpy(&m_FrontCamera.Name[0], "FrontCamera", sizeof(m_FrontCamera.Name));
    strncpy(&m_AnimalCamera.Name[0], "AnimalCamera", sizeof(m_AnimalCamera.Name));
    strncpy(&m_AroundCamera.Name[0], "AroundCamera", sizeof(m_AnimalCamera.Name));
    strncpy(&m_MotorMicon.Name[0], "Motor", sizeof(m_MotorMicon.Name));

    m_FrontCamera.PreviewCount = 0;
    m_AnimalCamera.PreviewCount = 0;
    m_AroundCamera.PreviewCount = 0;
    m_MotorMicon.PreviewCount = 0;

    m_FrontCamera.PreviewState = true;
    m_AnimalCamera.PreviewState = true;
    m_AroundCamera.PreviewState = true;
    m_MotorMicon.PreviewState = true;

    return ResultEnum::NormalEnd;
}

ResultEnum PatrolThread::doMainProc()
{
    bool systemError = false;

    /* �K�v�ȃX���b�h���s���I�����Ă��� */
    if (isThreadsOk() != true)
    {
        systemError = true;
    }

    /* �O���J���� �ʐM�m�F */
    if (isOtherConStateOk(pShareMemory->FrontCamera.ReceiveCount, &m_FrontCamera) != true)
    {
        systemError = true;
    }

    /* �����J���� �ʐM�m�F */
    if (isOtherConStateOk(pShareMemory->AnimalCamera.ReceiveCount, &m_AnimalCamera) != true)
    {
        systemError = true;
    }

    /* ���ӃJ���� �ʐM�m�F */
    if (isOtherConStateOk(pShareMemory->AroundCamera.ReceiveCount, &m_AroundCamera) != true)
    {
        systemError = true;
    }

    /* ���[�^�}�C�R�� �ʐM�m�F */
    if (isOtherConStateOk(pShareMemory->Motor.CommunicationCount, &m_MotorMicon) != true)
    {
        systemError = true;
    }

    pShareMemory->Commander.SystemError = systemError;

    return ResultEnum::NormalEnd;
}

ResultEnum PatrolThread::finalizeCore()
{
    /* �V���b�g�_�E�����ȊO�A���g���I�����邱�Ƃ͑z�肵�Ă��Ȃ� */
    if (pShareMemory != NULL)
    {
        pShareMemory->Commander.SystemError = true;
    }

    return ResultEnum::NormalEnd;
}

bool PatrolThread::isThreadsOk()
{
    bool retVal = true;
    bool state = false;

    for (list<ThreadInfo*>::iterator itr = m_TargetList.begin(); itr != m_TargetList.end(); ++itr)
    {
        ThreadInfo* target = *itr;

        ThreadBase::ThreadStateEnum threadState = target->Thread->GetState();

        if ((threadState != ThreadBase::ThreadStateEnum::NotStart)
        &&  (threadState != ThreadBase::ThreadStateEnum::Executing))
        {
            retVal = false;
            state = false;
        }
        else
        {
            state = true;
        }

        /* OK -> NG */
        if ((target->PreviewState == true) && (state == false))
        {
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[Patrol] Task[%s] Illegal state. state[%d]\n", &(target->Name[0]), threadState);
            m_Logger->LOG_ERROR(m_LogStr);
        }
        /* NG -> OK */
        else if ((target->PreviewState == false) && (state == true))
        {
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[Patrol] Task[%s] State OK return. state[%d]\n", &(target->Name[0]), threadState);
            m_Logger->LOG_INFO(m_LogStr);
        }
        else
        {
            /* nop. */
        }

        target->PreviewState = state;
    }

    return retVal;
}

bool PatrolThread::isOtherConStateOk(const long count, StateInfo* const info)
{
    bool retVal = true;

    /* ���ڑ� */
    if (count == 0)
    {
        info->PreviewCount = count;
        goto FINISH;
    }

    /* �ʐM�񐔂��X�V���ꂽ */
    if (count != info->PreviewCount)
    {
        info->Watch.Start();
        info->PreviewCount = count;
        goto FINISH;
    }

    /* �ʐM�񐔕ω��Ȃ� 5 �b�ȓ� */
    if (info->Watch.GetSplit() <= 5.0f)
    {
        goto FINISH;
    }

    /* ���}�C�R���Ƃ̒ʐM�� 5 �b�Ԏ��Ȃ� */
    retVal = false;

FINISH :

    /* OK -> NG */
    if ((info->PreviewState == true) && (retVal == false))
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[Patrol] %s-Con Connect Error. Count[%ld]\n", &(info->Name[0]), info->PreviewCount);
        m_Logger->LOG_ERROR(m_LogStr);
    }
    else if ((info->PreviewState == false) && (retVal == true))
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[Patrol] %s-Con Connect OK. Count[%ld]\n", &(info->Name[0]), info->PreviewCount);
        m_Logger->LOG_INFO(m_LogStr);
    }
    else
    {
        /* nop. */
    }

    info->PreviewState = retVal;

    return retVal;
}

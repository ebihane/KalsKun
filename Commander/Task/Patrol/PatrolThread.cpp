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

    /* 必要なスレッドが不正終了している */
    if (isThreadsOk() != true)
    {
        systemError = true;
    }

    /* 前方カメラ 通信確認 */
    if (isOtherConStateOk(pShareMemory->FrontCamera.ReceiveCount, &m_FrontCamera) != true)
    {
        systemError = true;
    }

    /* 動物カメラ 通信確認 */
    if (isOtherConStateOk(pShareMemory->AnimalCamera.ReceiveCount, &m_AnimalCamera) != true)
    {
        systemError = true;
    }

    /* 周辺カメラ 通信確認 */
    if (isOtherConStateOk(pShareMemory->AroundCamera.ReceiveCount, &m_AroundCamera) != true)
    {
        systemError = true;
    }

    /* モータマイコン 通信確認 */
    if (isOtherConStateOk(pShareMemory->Motor.CommunicationCount, &m_MotorMicon) != true)
    {
        systemError = true;
    }

    pShareMemory->Commander.SystemError = systemError;

    return ResultEnum::NormalEnd;
}

ResultEnum PatrolThread::finalizeCore()
{
    /* シャットダウン時以外、自身が終了することは想定していない */
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

    /* 未接続 */
    if (count == 0)
    {
        info->PreviewCount = count;
        goto FINISH;
    }

    /* 通信回数が更新された */
    if (count != info->PreviewCount)
    {
        info->Watch.Start();
        info->PreviewCount = count;
        goto FINISH;
    }

    /* 通信回数変化なし 5 秒以内 */
    if (info->Watch.GetSplit() <= 5.0f)
    {
        goto FINISH;
    }

    /* 他マイコンとの通信が 5 秒間取れない */
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

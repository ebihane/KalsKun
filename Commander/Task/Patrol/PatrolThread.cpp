#include <stdio.h>
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
        m_TargetList.push_back(target);
    }
}

ResultEnum PatrolThread::doMainProc()
{
    bool systemError = false;
    long motorCount = pShareMemory->Motor.CommunicationCount;
    long frontCameraCount = pShareMemory->FrontCamera.ReceiveCount;
    long animalCameraCount = pShareMemory->AnimalCamera.ReceiveCount;
    long aroundCameraCount = pShareMemory->AroundCamera.ReceiveCount;

    /* 必要なスレッドが不正終了している */
    int index = 0;
    for (list<ThreadBase*>::iterator itr = m_TargetList.begin(); itr != m_TargetList.end(); ++itr)
    {
        ThreadBase* target = *itr;
        ThreadBase::ThreadStateEnum state = target->GetState();
        if ((state != ThreadBase::ThreadStateEnum::NotStart) && (state != ThreadBase::ThreadStateEnum::Executing))
        {
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[Patrol] Task(%d) Illegal state. state[%d]\n", index, state);
            m_Logger->LOG_ERROR(m_LogStr);
            systemError = true;
        }
        index++;
    }

    if (frontCameraCount == 0)
    {
        m_PrevFrontCameraCount = frontCameraCount;
    }
    else if (frontCameraCount != m_PrevFrontCameraCount)
    {
        m_FrontCameraWatch.Start();
        m_PrevFrontCameraCount = frontCameraCount;
    }
    else if (m_FrontCameraWatch.GetSplit() <= 5.0f)
    {
        /* nop. */
    }
    else
    {
        /* 前方カメラマイコンとの通信が 5 秒間取れない */
        systemError = true;
    }

    if (animalCameraCount == 0)
    {
        m_PrevAnimalCameraCount = animalCameraCount;
    }
    else if (animalCameraCount != m_PrevAnimalCameraCount)
    {
        m_AnimalCameraWatch.Start();
        m_PrevAnimalCameraCount = animalCameraCount;
    }
    else if (m_AnimalCameraWatch.GetSplit() <= 5.0f)
    {
        /* nop. */
    }
    else
    {
        /* 動物カメラマイコンとの通信が 5 秒間取れない */
        systemError = true;
    }

    if (aroundCameraCount == 0)
    {
        m_PrevAroundCameraCount = aroundCameraCount;
    }
    else if (aroundCameraCount != m_PrevAroundCameraCount)
    {
        m_AroundCameraWatch.Start();
        m_PrevAroundCameraCount = aroundCameraCount;
    }
    else if (m_AroundCameraWatch.GetSplit() <= 5.0f)
    {
        /* nop. */
    }
    else
    {
        /* 周辺カメラマイコンとの通信が 5 秒間取れない */
        systemError = true;
    }

    if (motorCount == 0)
    {
        m_PrevMotorCount = motorCount;
    }
    else if (motorCount != m_PrevMotorCount)
    {
        m_MotorWatch.Start();
        m_PrevMotorCount = motorCount;
    }
    else if (m_MotorWatch.GetSplit() <= 5.0f)
    {
        /* nop. */
    }
    else
    {
        /* モータマイコンとの通信が 5 秒間取れない */
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
        pShareMemory->Commander.SystemError = 1;
    }

    return ResultEnum::NormalEnd;
}

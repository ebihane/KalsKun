#include <stdio.h>
#include "LoopThreadBase.h"

LoopThreadBase::LoopThreadBase(char* const taskName, const unsigned long cycle, const TypeEnum type)
 : ThreadBase(taskName)
 , LOOP_CYCLE(cycle)
 , PROC_TYPE(type)
{
    /* nop. */
}

LoopThreadBase::~LoopThreadBase()
{
    finalize();
}

ResultEnum LoopThreadBase::initialize()
{
    ResultEnum retVal = initializeCore();
    return retVal;
}

ResultEnum LoopThreadBase::doProcedure()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    float elapsedTime = 0.0F;
    unsigned long elapsedMsec = 0;
    unsigned long waitTime = 0;


    snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] Main loop enter.\n", m_TaskName);
    m_Logger->LOG_INFO(m_LogStr);

    while (1)
    {
        if (isStopRequest() == true)
        {
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] Thread stop request.\n", m_TaskName);
            m_Logger->LOG_INFO(m_LogStr);
            break;
        }

        m_Watch.Start();
        if (doMainProc() != ResultEnum::NormalEnd)
        {
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] doMainProc failed.\n", m_TaskName);
            m_Logger->LOG_INFO(m_LogStr);
            goto FINISH;
        }
        m_Watch.Stop();

        elapsedTime = m_Watch.Get();
        elapsedMsec = (unsigned long)(elapsedTime * 1000.0F);

        if (PROC_TYPE == TypeEnum::TIMER_STOP)
        {
            waitTime = LOOP_CYCLE;
        }
        else if (LOOP_CYCLE <= elapsedMsec)
        {
            waitTime = 1;
        }
        else
        {
            waitTime = LOOP_CYCLE - elapsedMsec;
        }

        delay(waitTime);
    }

    snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] Main loop exit.\n", m_TaskName);
    m_Logger->LOG_INFO(m_LogStr);

FINISH :
    return retVal;
}

ResultEnum LoopThreadBase::finalize()
{
    ResultEnum retVal = finalizeCore();
    return retVal;
}

ResultEnum LoopThreadBase::initializeCore()
{
    return ResultEnum::NormalEnd;
}

ResultEnum LoopThreadBase::finalizeCore()
{
    return ResultEnum::NormalEnd;
}

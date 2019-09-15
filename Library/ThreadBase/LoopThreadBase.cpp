#include "LoopThreadBase.h"

LoopThreadBase::LoopThreadBase(const unsigned long cycle)
 : LOOP_CYCLE(cycle)
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

    m_Logger->LOG_INFO("[doProcedure] Main loop enter.\n");
    while (1)
    {
        if (isStopRequest() == true)
        {
            m_Logger->LOG_INFO("[doProcedure] Thread stop request.\n");
            break;
        }

        m_Watch.Start();
        if (doMainProc() != ResultEnum::NormalEnd)
        {
            m_Logger->LOG_ERROR("[doProcedure] doMainProc failed.\n");
            goto FINISH;
        }
        m_Watch.Stop();

        elapsedTime = m_Watch.Get();
        elapsedMsec = (unsigned long)(elapsedTime * 1000.0F);

        if (LOOP_CYCLE <= elapsedMsec)
        {
            waitTime = 1;
        }
        else
        {
            waitTime = LOOP_CYCLE - elapsedMsec;
        }

        delay(waitTime);
    }
    m_Logger->LOG_INFO("[doProcedure] Main loop exit.\n");

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

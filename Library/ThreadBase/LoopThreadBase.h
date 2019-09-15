#pragma once

#include "Measure/Stopwatch.h"
#include "ThreadBase/ThreadBase.h"

/* ˆê’èüŠú‚Åˆ—‚ğÀ{‚·‚é‚½‚ß‚ÌƒNƒ‰ƒX */
class LoopThreadBase : public ThreadBase
{
public :

    LoopThreadBase(const unsigned long cycle);
    virtual ~LoopThreadBase();

protected :

    virtual ResultEnum initializeCore();
    virtual ResultEnum doMainProc() = 0;
    virtual ResultEnum finalizeCore();

private :

    const unsigned long LOOP_CYCLE;

    Stopwatch m_Watch;

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

};

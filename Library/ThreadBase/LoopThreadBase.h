#pragma once

#include "Measure/Stopwatch.h"
#include "ThreadBase/ThreadBase.h"

/* ˆê’èüŠú‚Åˆ—‚ğÀ{‚·‚é‚½‚ß‚ÌƒNƒ‰ƒX */
class LoopThreadBase : public ThreadBase
{
public :

    typedef enum
    {
        CYCLIC = 0,
        TIMER_STOP,
    } TypeEnum;

    LoopThreadBase(const unsigned long cycle, const TypeEnum type);
    virtual ~LoopThreadBase();

protected :

    virtual ResultEnum initializeCore();
    virtual ResultEnum doMainProc() = 0;
    virtual ResultEnum finalizeCore();

private :

    const unsigned long LOOP_CYCLE;
    const TypeEnum PROC_TYPE;

    Stopwatch m_Watch;

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

};

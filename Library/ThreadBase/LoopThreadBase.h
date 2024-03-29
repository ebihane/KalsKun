#pragma once

#include "Measure/Stopwatch.h"
#include "ThreadBase/ThreadBase.h"

/* 一定周期で処理を実施するためのクラス */
class LoopThreadBase : public ThreadBase
{
public :

    typedef enum
    {
        CYCLIC = 0,
        TIMER_STOP,
    } TypeEnum;

    LoopThreadBase(char* const taskName, const unsigned long cycle, const TypeEnum type);
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

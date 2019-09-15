#pragma once

#include "Measure/Stopwatch.h"
#include "ThreadBase/ThreadBase.h"

/* 一定周期で処理を実施するためのクラス */
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

#pragma once

#include "Measure/Stopwatch.h"
#include "ThreadBase/ThreadBase.h"

/* �������ŏ��������{���邽�߂̃N���X */
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

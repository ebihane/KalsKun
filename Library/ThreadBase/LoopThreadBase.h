#pragma once

#include "Measure/Stopwatch.h"
#include "ThreadBase/ThreadBase.h"

/* �������ŏ��������{���邽�߂̃N���X */
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

#pragma once

#include <list>
#include "ThreadBase/LoopThreadBase.h"

class PatrolThread : public LoopThreadBase
{
public :

    PatrolThread();
    virtual ~PatrolThread();

    void AddTarget(ThreadBase* const target);

protected : 

private :

    std::list<ThreadBase*> m_TargetList;

    long m_PrevFrontCameraCount;
    long m_PrevAnimalCameraCount;
    long m_PrevAroundCameraCount;
    long m_PrevMotorCount;
    Stopwatch m_FrontCameraWatch;
    Stopwatch m_AnimalCameraWatch;
    Stopwatch m_AroundCameraWatch;
    Stopwatch m_MotorWatch;

    ResultEnum doMainProc();
    ResultEnum finalizeCore();

};

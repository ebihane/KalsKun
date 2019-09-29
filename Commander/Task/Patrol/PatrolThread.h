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
    
    /* スレッド情報構造体 */
    typedef struct
    {
        char Name[16];
        bool PreviewState;
        ThreadBase* Thread;
    } ThreadInfo;

    /*  */
    typedef struct
    {
        char Name[20];
        bool PreviewState;
        long PreviewCount;
        Stopwatch Watch;
    } StateInfo;

    std::list<ThreadInfo*> m_TargetList;

    StateInfo   m_FrontCamera;
    StateInfo   m_AnimalCamera;
    StateInfo   m_AroundCamera;
    StateInfo   m_MotorMicon;

    ResultEnum initializeCore();
    ResultEnum doMainProc();
    ResultEnum finalizeCore();

    bool isThreadsOk();
    bool isOtherConStateOk(const long count, StateInfo* const info);

};

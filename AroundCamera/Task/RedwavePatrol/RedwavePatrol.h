#include "ThreadBase/LoopThreadBase.h"

class RedwavePatrol : public LoopThreadBase
{
public :

    RedwavePatrol();
    virtual ~RedwavePatrol();

protected :

private :

    bool m_BeforeDetected;
    Stopwatch m_NormalReturnWatch;

    ResultEnum initializeCore();
    ResultEnum doMainProc();
    ResultEnum finalizeCore();

};

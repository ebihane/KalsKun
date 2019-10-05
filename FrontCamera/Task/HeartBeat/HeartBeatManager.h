#include "ThreadBase/LoopThreadBase.h"

class HeartBeatManager : public LoopThreadBase
{
public :

    HeartBeatManager();
    virtual ~HeartBeatManager();

protected :


private :

    int m_CurrentLevel;

    ResultEnum doMainProc();
    ResultEnum finalizeCore();

};

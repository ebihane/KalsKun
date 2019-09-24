#pragma once

#include "ThreadBase/LoopThreadBase.h"
#include "Parts/ShareMemory/ShareMemory.h"

class LightController : public LoopThreadBase
{
public :

    LightController();
    virtual ~LightController();

protected :


private :

    bool    m_OnOff;

    ResultEnum initializeCore();
    ResultEnum doMainProc();
    ResultEnum finalizeCore();

};

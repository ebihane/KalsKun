#pragma once

#include "ThreadBase/LoopThreadBase.h"

class ErrorLedManager : public LoopThreadBase
{
public :

    ErrorLedManager();
    virtual ~ErrorLedManager();

protected :


private :

    int m_CurrentLevel;

    ResultEnum doMainProc();

};

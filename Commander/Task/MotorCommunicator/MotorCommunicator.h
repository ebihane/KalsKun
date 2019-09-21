#pragma once

#include "Serial/Serial.h"
#include "ThreadBase/LoopThreadBase.h"

class MotorCommunicator : public LoopThreadBase
{
public :

    MotorCommunicator();
    virtual ~MotorCommunicator();

protected :


private :

    Serial* m_Serial;

    ResultEnum initializeCore();
    ResultEnum doMainProc();
    ResultEnum finalizeCore();

};
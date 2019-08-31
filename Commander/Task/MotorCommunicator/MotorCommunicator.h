#pragma once

#include "ThreadBase/SenderThread.h"

class MotorCommunicator : public SenderThread
{
public :

    MotorCommunicator(AdapterBase* const adapter);
    virtual ~MotorCommunicator();

protected :


private :

    bool m_SendTiming;

    ResultEnum initializeCore();
    bool createSendData(char* const data, unsigned long* size);

};
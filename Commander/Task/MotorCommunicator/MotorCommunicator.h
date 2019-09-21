#pragma once

#include "Serial/Serial.h"
#include "ThreadBase/ReceiverThread.h"

class MotorCommunicator : public ReceiverThread
{
public :

    MotorCommunicator(AdapterBase* const adapter);
    virtual ~MotorCommunicator();

protected :


private :

    Serial* m_Serial;
    bool    m_SendTiming;

    ResultEnum initializeCore();
    bool isReceiveComplete(char* const buffer, const unsigned long size);
    ResultEnum analyze(char* const buffer);

};
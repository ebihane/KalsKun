#pragma once

#include "ThreadBase/SenderThread.h"

class StateSender : public SenderThread
{
public :

    StateSender(AdapterBase* const adapter);
    virtual ~StateSender();

protected :

private :

    bool m_SendTiming;
    long m_SendCount;

    ResultEnum initializeCore();
    bool createSendData(char* const data, unsigned long* const size);

};

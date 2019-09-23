#pragma once

#include "Adapter/AdapterBase.h"
#include "ThreadBase/SenderThread.h"

class StateSender : public SenderThread
{
public :

    StateSender(AdapterBase* const adapter);
    virtual ~StateSender();

protected :

private :

    long m_SendCount;

    ResultEnum initializeCore();
    bool createSendData(char* const data, unsigned long* const size);

};

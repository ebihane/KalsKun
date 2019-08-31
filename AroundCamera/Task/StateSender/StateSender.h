#pragma once

#include "ThreadBase/SenderThread.h"

class StateSender : public SenderThread
{
public :

    StateSender(AdapterBase* const adapter);
    virtual ~StateSender();

protected :

private :

    long m_SendCount;

    bool createSendData(EventInfo* const ev);

};

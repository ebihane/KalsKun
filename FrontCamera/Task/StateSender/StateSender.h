#pragma once

#include "Adapter/AdapterBase.h"
#include "ThreadBase/SenderThread.h"

class StateSender : public SenderThread
{
public :

    StateSender(AdapterBase* const adapter);
    virtual ~StateSender();
	void SetState(long moveCommand);

protected :

private :

    long m_SendCount;
	long m_MoveCommand;

    ResultEnum initializeCore();
    bool createSendData(char* const data, unsigned long* const size);

};

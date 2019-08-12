#pragma once

#include "Socket/TcpClient/TcpClient.h"
#include "Queue/Queue.h"
#include "ThreadBase/ThreadBase.h"

class SenderThread : public ThreadBase
{
public :

    SenderThread(char* const ipAddress, const unsigned short portNo);
    virtual ~SenderThread();

protected :

private :

    TcpClient* m_TcpClient;
    Queue* m_Queue;

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

};

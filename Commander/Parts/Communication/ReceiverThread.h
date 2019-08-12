#pragma once

#include "Socket/TcpServer/TcpServer.h"
#include "Queue/Queue.h"
#include "ThreadBase/ThreadBase.h"

class ReceiverThread : public ThreadBase
{
public :

    ReceiverThread(const unsigned short portNo);
    virtual ~ReceiverThread();

protected :

private :

    TcpServer* m_TcpServer;
    Queue* m_Queue;

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

};

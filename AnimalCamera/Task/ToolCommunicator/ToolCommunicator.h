#pragma once

#include "Socket/TcpServer/TcpServer.h"
#include "ThreadBase/ThreadBase.h"

class ToolCommunicator : public ThreadBase
{
public:

    ToolCommunicator();
    virtual ~ToolCommunicator();

protected:


private:

    TcpServer  m_TcpServer;

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

    ResultEnum analyzeAndExecuteEvent(EventInfo* const pEv);
    ResultEnum doDateTimeAdjust(EventInfo* const pEv);
    ResultEnum doDisconnectEvent();

};

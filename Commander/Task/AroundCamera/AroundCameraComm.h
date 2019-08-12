#pragma once

#include "ThreadBase/ThreadBase.h"
#include "Socket/TcpServer/TcpServer.h"

class AroundCameraComm : public ThreadBase
{
public :

    AroundCameraComm();
    virtual ~AroundCameraComm();

protected :

private :

    TcpServer m_TcpServer;

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

};
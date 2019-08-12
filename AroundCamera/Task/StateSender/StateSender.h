#pragma once

#include "Socket/TcpClient/TcpClient.h"
#include "ThreadBase/ThreadBase.h"

class StateSender : public ThreadBase
{
public :

    StateSender();
    virtual ~StateSender();

protected :

private :

    TcpClient m_TcpClient;

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

};

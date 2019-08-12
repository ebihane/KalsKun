#pragma once

#include "ThreadBase/ThreadBase.h"
#include "Logger/Logger.h"
#include "Socket/UdpSender/UdpSender.h"

class CameraSender : public ThreadBase
{
public :

    CameraSender();
    virtual ~CameraSender();

protected :


private :

    UdpSender* m_UdpSender;
    char* m_SendBuffer;

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

};

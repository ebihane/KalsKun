#pragma once

#include "ThreadBase/ThreadBase.h"
#include "Logger/Logger.h"
#include "Socket/UdpReceiver/UdpReceiver.h"

#define UDP_RECEIVE_MAX    (65500)

class CameraReceiver : public ThreadBase
{
public :

    CameraReceiver();
    virtual ~CameraReceiver();

protected :


private :

    UdpReceiver* m_UdpReceiver;
    char* m_ReceiveBuffer;

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

};
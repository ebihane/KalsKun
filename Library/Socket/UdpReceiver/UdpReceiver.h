#pragma once
#include "../SocketBase.h"

class UdpReceiver : public SocketBase
{
public :

    UdpReceiver(const unsigned short portNo);
    virtual ~UdpReceiver();

    ResultEnum Open();
    ResultEnum Close();
    ResultEnum Connection();
    ResultEnum Disconnection();
    ResultEnum IsSendable(bool& sendable);
    ResultEnum Send(void* const bufferPtr, const unsigned long size);
    ResultEnum IsReceivable(bool& receivable);
    ResultEnum Receive(void* const bufferPtr, const unsigned long size);

protected :


private :

    int m_Socket;

};


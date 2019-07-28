#pragma once
#include "../SocketBase.h"

class TcpServer : public SocketBase
{
public :
    static const int    SOCKET_INVALID = -1;

    TcpServer(const unsigned short portNo);
    virtual ~TcpServer();

    ResultEnum Open();
    ResultEnum Close();
    ResultEnum Connection();
    ResultEnum Disconnection();
    ResultEnum IsSendable(bool& sendable);
    ResultEnum Send(void* const bufferPtr, const unsigned long size);
    ResultEnum IsReceivable(bool& receivable);
    ResultEnum Receive(void* const bufferPtr, const unsigned long size);

private :

    int m_OpenSocket;
    int m_ConnectSocket;

};

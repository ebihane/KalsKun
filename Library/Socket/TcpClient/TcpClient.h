#pragma once
#include "../SocketBase.h"

class TcpClient : public SocketBase
{
public:
    TcpClient(char* const ipAddress, const unsigned short portNo);
	virtual ~TcpClient();

    ResultEnum Open();
    ResultEnum Close();
    ResultEnum Connection();
    ResultEnum Disconnection();
    ResultEnum IsSendable(bool& sendable);
    ResultEnum Send(void* const bufferPtr, const unsigned long size);
    ResultEnum IsReceivable(bool& receivable);
    ResultEnum Receive(void* const bufferPtr, const unsigned long size);

private :

    static const long   IPADDRESS_LENGTH = 16;

    int             m_Socket;
	char            m_IpAddress[IPADDRESS_LENGTH];

};

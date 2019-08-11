#pragma once
#include <netinet/in.h>
#include "../SocketBase.h"

#define UDP_SEND_MAX    (65500)

class UdpSender : public SocketBase
{
public :

    UdpSender(char* const ipAddress, const unsigned short portNo, const bool bloadCastSendable);
    virtual ~UdpSender();

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

    static const long   IPADDRESS_LENGTH = 16;

    int m_Socket;
    bool m_BroadcastSendable;
    char m_IpAddress[IPADDRESS_LENGTH];
    struct sockaddr_in m_Addr;

};

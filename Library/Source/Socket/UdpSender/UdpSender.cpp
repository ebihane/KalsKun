#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "UdpSender.h"

UdpSender::UdpSender(char* const ipAddress, const unsigned short portNo, const bool bloadCastSendable)
 : SocketBase(portNo)
 , m_Socket(SOCKET_INVALID)
 , m_BroadcastSendable(bloadCastSendable)
{
    memset(&m_Addr, 0x00, sizeof(m_Addr));
    memcpy(&m_IpAddress[0], &ipAddress[0], IPADDRESS_LENGTH);
}

UdpSender::~UdpSender()
{
    Close();
}

ResultEnum UdpSender::Open()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    int sock = SOCKET_INVALID;
    struct sockaddr_in addr;
    int yes = 1;


    Disconnection();

    m_LastErrorNo = 0;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_NO);

    if (m_BroadcastSendable == true)
    {
        addr.sin_addr.s_addr = inet_addr("255.255.255.255");
    }
    else
    {
        addr.sin_addr.s_addr = inet_addr(&m_IpAddress[0]);
    }

    if (m_BroadcastSendable == true)
    {
        m_LastErrorNo = errno;
        if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)& yes, sizeof(yes)) < 0)
        {
            m_LastErrorNo = errno;
            goto FINISH;
        }
    }

    m_Socket = sock;
    m_Addr = addr;

    retVal = ResultEnum::NormalEnd;


FINISH :
    return retVal;
}

ResultEnum UdpSender::Close()
{
    ResultEnum retVal = Disconnection();
    return retVal;
}

ResultEnum UdpSender::Connection()
{
    return ResultEnum::NormalEnd;
}

ResultEnum UdpSender::Disconnection()
{
    if (m_Socket != SOCKET_INVALID)
    {
        close(m_Socket);
        m_Socket = SOCKET_INVALID;
    }

    return ResultEnum::NormalEnd;
}

ResultEnum UdpSender::IsSendable(bool& sendable)
{
    ResultEnum retVal = isSendableCore(m_Socket, sendable);
    return retVal;
}

ResultEnum UdpSender::Send(void* const bufferPtr, const unsigned long size)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;


    m_LastErrorNo = ERROR_NOTHING;
    if (sendto(m_Socket, bufferPtr, size, 0, (struct sockaddr *)&m_Addr, sizeof(m_Addr)) < 0)
    {
        m_LastErrorNo = errno;
        retVal = ResultEnum::Reconnect;
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;

FINISH:
    return retVal;
}

ResultEnum UdpSender::IsReceivable(bool& receivable)
{
    receivable = false;
    return ResultEnum::AbnormalEnd;
}

ResultEnum UdpSender::Receive(void* const bufferPtr, const unsigned long size)
{
    return ResultEnum::AbnormalEnd;
}

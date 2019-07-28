#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include "UdpReceiver.h"

UdpReceiver::UdpReceiver(const unsigned short portNo)
 : SocketBase(portNo)
 , m_Socket(SOCKET_INVALID)
{
    /* nop. */
}

UdpReceiver::~UdpReceiver()
{
    Close();
}

ResultEnum UdpReceiver::Open()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    int sock = SOCKET_INVALID;
    struct sockaddr_in addr;


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
    addr.sin_addr.s_addr = INADDR_ANY;

    m_LastErrorNo = 0;
    if (bind(sock, (struct sockaddr*) & addr, sizeof(addr)) < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    m_Socket = sock;
    
    retVal = ResultEnum::NormalEnd;


FINISH :
    return retVal;
}

ResultEnum UdpReceiver::Close()
{
    ResultEnum retVal = Disconnection();
    return retVal;
}

ResultEnum UdpReceiver::Connection()
{
    return ResultEnum::NormalEnd;
}

ResultEnum UdpReceiver::Disconnection()
{
    if (m_Socket != SOCKET_INVALID)
    {
        close(m_Socket);
        m_Socket = SOCKET_INVALID;
    }

    return ResultEnum::NormalEnd;
}

ResultEnum UdpReceiver::IsSendable(bool& sendable)
{
    sendable = false;
    return ResultEnum::AbnormalEnd;
}

ResultEnum UdpReceiver::Send(void* const bufferPtr, const unsigned long size)
{
    return ResultEnum::AbnormalEnd;
}

ResultEnum UdpReceiver::IsReceivable(bool& receivable)
{
    ResultEnum retVal = isReceivableCore(m_Socket, receivable);
    return retVal;
}

ResultEnum UdpReceiver::Receive(void* const bufferPtr, const unsigned long size)
{
    ResultEnum retVal = receiveCore(m_Socket, bufferPtr, size);
    return retVal;
}

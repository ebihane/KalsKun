#include <wiringPi.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "TcpClient.h"

TcpClient::TcpClient(char* const ipAddress, const unsigned short portNo)
 : SocketBase(portNo)
 , m_Socket(SOCKET_INVALID)
{
    memcpy(&m_IpAddress[0], &ipAddress[0], IPADDRESS_LENGTH);
}

TcpClient::~TcpClient()
{
    Close();
}

ResultEnum TcpClient::Open()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    int sock = SOCKET_INVALID;

    Disconnection();

    m_LastErrorNo = ERROR_NOTHING;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    m_Socket = sock;

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

ResultEnum TcpClient::Close()
{
    ResultEnum retVal = Disconnection();
    return retVal;
}

ResultEnum TcpClient::Connection()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    struct sockaddr_in addr;


    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(&m_IpAddress[0]);
    addr.sin_port = htons(PORT_NO);

    m_LastErrorNo = ERROR_NOTHING;
    if (connect(m_Socket, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        m_LastErrorNo = errno;
        retVal = ResultEnum::Reconnect;
        goto FINISH;
    }

    delay(5000);

    retVal = ResultEnum::NormalEnd;

FINISH:
    return retVal;
}

ResultEnum TcpClient::Disconnection()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    if (m_Socket == SOCKET_INVALID)
    {
        retVal = ResultEnum::NormalEnd;
        goto FINISH;
    }

    m_LastErrorNo = ERROR_NOTHING;
    if (close(m_Socket) < 0)
    {
        m_LastErrorNo = errno;
        retVal = ResultEnum::Reconnect;
        goto FINISH;
    }

    m_Socket = SOCKET_INVALID;

    retVal = ResultEnum::NormalEnd;

FINISH:
    return retVal;
}

ResultEnum TcpClient::IsSendable(bool& sendable)
{
    ResultEnum retVal = isSendableCore(m_Socket, sendable);
    return retVal;
}

ResultEnum TcpClient::Send(void* const bufferPtr, const unsigned long size)
{
    ResultEnum retVal = sendCore(m_Socket, bufferPtr, size);
    return retVal;
}

ResultEnum TcpClient::IsReceivable(bool& receivable)
{
    ResultEnum retVal = isReceivableCore(m_Socket, receivable);
    return retVal;
}

ResultEnum TcpClient::Receive(void* const bufferPtr, const unsigned long size)
{
    ResultEnum retVal = receiveCore(m_Socket, bufferPtr, size);
    return retVal;
}

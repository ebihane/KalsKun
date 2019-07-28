#include <wiringPi.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "TcpServer.h"

TcpServer::TcpServer(const unsigned short portNo)
 : SocketBase(portNo)
 , m_OpenSocket(SOCKET_INVALID)
 , m_ConnectSocket(SOCKET_INVALID)
{
    /* nop. */
}

TcpServer::~TcpServer()
{
    Close();
}

ResultEnum TcpServer::Open()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    int sock = SOCKET_INVALID;
    int yes = 1;
    struct sockaddr_in addr;


    Disconnection();

    m_LastErrorNo = ERROR_NOTHING;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_NO);
    addr.sin_addr.s_addr = INADDR_ANY;

    m_LastErrorNo = ERROR_NOTHING;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes)) < 0)
    {
        m_LastErrorNo = errno;
        close(sock);
        goto FINISH;
    }

    m_LastErrorNo = errno;
    if (bind(sock, (struct sockaddr*) & addr, sizeof(addr)) < 0)
    {
        m_LastErrorNo = errno;
        close(sock);
        goto FINISH;
    }

    m_LastErrorNo = errno;
    if (listen(sock, 5) < 0)
    {
        m_LastErrorNo = errno;
        close(sock);
        goto FINISH;
    }


    m_OpenSocket = sock;

    retVal = ResultEnum::NormalEnd;


FINISH :
    return retVal;
}

ResultEnum TcpServer::Close()
{
    Disconnection();

    if (m_OpenSocket != SOCKET_INVALID)
    {
        close(m_OpenSocket);
        m_OpenSocket = SOCKET_INVALID;
    }

    return ResultEnum::NormalEnd;
}

ResultEnum TcpServer::Connection()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    int sock = SOCKET_INVALID;

    while (1)
    {
        m_LastErrorNo = 0;
        sock = accept(m_OpenSocket, NULL, NULL);
        if (0 <= sock)
        {
            break;
        }

        if ((errno == EAGAIN)
        ||  (errno == EWOULDBLOCK)
        ||  (errno == EINTR))
        {
            delay(3000);
            continue;
        }
        else
        {
            retVal = ResultEnum::Reconnect;
            goto FINISH;
        }
    }

    m_ConnectSocket = sock;

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

ResultEnum TcpServer::Disconnection()
{
    if (m_ConnectSocket != SOCKET_INVALID)
    {
        close(m_ConnectSocket);
        m_ConnectSocket = SOCKET_INVALID;
    }

    return ResultEnum::NormalEnd;
}

ResultEnum TcpServer::IsSendable(bool& sendable)
{
    ResultEnum retVal = isSendableCore(m_ConnectSocket, sendable);
    return retVal;
}

ResultEnum TcpServer::Send(void* const bufferPtr, const unsigned long size)
{
    ResultEnum retVal = sendCore(m_ConnectSocket, bufferPtr, size);
    return retVal;
}

ResultEnum TcpServer::IsReceivable(bool& receivable)
{
    ResultEnum retVal = isReceivableCore(m_ConnectSocket, receivable);
    return retVal;
}

ResultEnum TcpServer::Receive(void* const bufferPtr, const unsigned long size)
{
    ResultEnum retVal = receiveCore(m_ConnectSocket, bufferPtr, size);
    return retVal;
}

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "SocketBase.h"

SocketBase::SocketBase(const unsigned short portNo)
 : AdapterBase()
 , PORT_NO(portNo)
{
    /* nop. */
}

SocketBase::~SocketBase()
{
}

int SocketBase::GetLastError()
{
    return m_LastErrorNo;
}

ResultEnum SocketBase::isSendableCore(const int sock, bool& sendable)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    fd_set sendFds;
    fd_set errorFds;
    struct timeval timeout;

    sendable = false;

    if (sock == SOCKET_INVALID)
    {
        retVal = ResultEnum::Reconnect;
        goto FINISH;
    }

    FD_ZERO(&sendFds);
    FD_ZERO(&errorFds);
    FD_SET(sock, &sendFds);
    FD_SET(sock, &errorFds);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    m_LastErrorNo = ERROR_NOTHING;
    if (select(sock + 1, NULL, &sendFds, &errorFds, &timeout) < 0)
    {
        m_LastErrorNo = errno;
        retVal = ResultEnum::Reconnect;
    }
    else if (FD_ISSET(sock, &errorFds) != 0)
    {
        m_LastErrorNo = errno;
        retVal = ResultEnum::Reconnect;
    }
    else if (FD_ISSET(sock, &sendFds) == 0)
    {
        retVal = ResultEnum::NormalEnd;
    }
    else
    {
        sendable = true;
        retVal = ResultEnum::NormalEnd;
    }

FINISH:
    return retVal;
}

ResultEnum SocketBase::sendCore(const int sock, void* const bufferPtr, const unsigned long size)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    int onceSend = 0;
    unsigned long rest = size;
    char* ptr = (char*)bufferPtr;

    if (sock == SOCKET_INVALID)
    {
        retVal = ResultEnum::Reconnect;
        goto FINISH;
    }

    m_LastErrorNo = ERROR_NOTHING;
    while (0 < rest)
    {
        onceSend = send(sock, ptr, rest, 0);
        if (onceSend < 0)
        {
            m_LastErrorNo = errno;
            retVal = ResultEnum::Reconnect;
            goto FINISH;
        }

        ptr += onceSend;
        rest -= onceSend;
    }

    retVal = ResultEnum::NormalEnd;

FINISH:
    return retVal;
}

ResultEnum SocketBase::isReceivableCore(const int sock, bool& receivable)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    fd_set recvFds;
    fd_set errorFds;
    struct timeval  timeout;

    receivable = false;

    if (sock == SOCKET_INVALID)
    {
        retVal = ResultEnum::Reconnect;
        goto FINISH;
    }

    FD_ZERO(&recvFds);
    FD_ZERO(&errorFds);
    FD_SET(sock, &recvFds);
    FD_SET(sock, &errorFds);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    m_LastErrorNo = ERROR_NOTHING;
    if (select(sock + 1, &recvFds, NULL, &errorFds, &timeout) < 0)
    {
        m_LastErrorNo = errno;
        retVal = ResultEnum::Reconnect;
    }
    else if (FD_ISSET(sock, &errorFds) != 0)
    {
        m_LastErrorNo = errno;
        retVal = ResultEnum::Reconnect;
    }
    else if (FD_ISSET(sock, &recvFds) == 0)
    {
        retVal = ResultEnum::NormalEnd;
    }
    else
    {
        receivable = true;
        retVal = ResultEnum::NormalEnd;
    }

FINISH:
    return retVal;
}

ResultEnum SocketBase::receiveCore(const int sock, void* const bufferPtr, const unsigned long size)
{
    ResultEnum   retVal = ResultEnum::AbnormalEnd;
    int onceReceive = 0;
    unsigned long rest = size;
    char* ptr = (char*)bufferPtr;

    if (sock == SOCKET_INVALID)
    {
        retVal = ResultEnum::Reconnect;
        goto FINISH;
    }

    m_LastErrorNo = ERROR_NOTHING;
    while (0 < rest)
    {
        onceReceive = recv(sock, ptr, rest, 0);
        if (onceReceive < 0)
        {
            m_LastErrorNo = errno;
            retVal = ResultEnum::Reconnect;
            goto FINISH;
        }

        ptr += onceReceive;
        rest -= onceReceive;
    }


    retVal = ResultEnum::NormalEnd;

FINISH:
    return retVal;
}

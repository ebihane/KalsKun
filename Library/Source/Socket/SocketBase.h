#pragma once
#include "Include/Common.h"

class SocketBase
{
public:
    static const int    SOCKET_INVALID = -1;
    static const int    ERROR_NOTHING = -1;

    SocketBase(const unsigned short portNo);
    virtual ~SocketBase();

    virtual ResultEnum Open() = 0;
    virtual ResultEnum Close() = 0;
    virtual ResultEnum Connection() = 0;
    virtual ResultEnum Disconnection() = 0;
    
    virtual ResultEnum IsSendable(bool& sendable) = 0;
    virtual ResultEnum Send(void* const bufferPtr, const unsigned long size) = 0;
    virtual ResultEnum IsReceivable(bool& receivable) = 0;
    virtual ResultEnum Receive(void* const bufferPtr, const unsigned long size) = 0;

    int GetLastError();

protected :

    const unsigned short    PORT_NO;

    int m_LastErrorNo;

    ResultEnum isSendableCore(const int sock, bool& sendable);
    ResultEnum sendCore(const int sock, void* const bufferPtr, const unsigned long size);
    ResultEnum isReceivableCore(const int sock, bool& receivable);
    ResultEnum receiveCore(const int sokc, void* const bufferPtr, const unsigned long size);

private :

};

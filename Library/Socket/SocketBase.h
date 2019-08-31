#pragma once
#include "Include/Common.h"
#include "Adapter/AdapterBase.h"

class SocketBase : public AdapterBase
{
public:

    static const int SOCKET_INVALID = -1;

    SocketBase(const unsigned short portNo);
    virtual ~SocketBase();

    int GetLastError();

protected :

    const unsigned short    PORT_NO;

    ResultEnum isSendableCore(const int sock, bool& sendable);
    ResultEnum sendCore(const int sock, void* const bufferPtr, const unsigned long size);
    ResultEnum isReceivableCore(const int sock, bool& receivable);
    ResultEnum receiveCore(const int sokc, void* const bufferPtr, const unsigned long size);

private :

};

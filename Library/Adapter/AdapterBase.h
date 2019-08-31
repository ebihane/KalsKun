#pragma once
#include "Include/Common.h"

class AdapterBase
{
public:

    static const int    ERROR_NOTHING = 0;

    AdapterBase()
    : m_LastErrorNo(ERROR_NOTHING)
    {
        /* nop. */
    };

    virtual ~AdapterBase() {};

    virtual ResultEnum Open() = 0;
    virtual ResultEnum Close() = 0;
    virtual ResultEnum Connection() = 0;
    virtual ResultEnum Disconnection() = 0;

    virtual ResultEnum IsSendable(bool& sendable) = 0;
    virtual ResultEnum Send(void* const bufferPtr, const unsigned long size) = 0;
    virtual ResultEnum IsReceivable(bool& receivable) = 0;
    virtual ResultEnum Receive(void* const bufferPtr, const unsigned long size) = 0;

    inline int GetLastError()
    {
        return m_LastErrorNo;
    }

protected:

    int m_LastErrorNo = 0;

private:

};

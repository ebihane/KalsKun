#pragma once

#include "Include/Common.h"

class SendQueue
{
public:

    static const int INVALID_QUEUE = -1;
    static const int ERROR_NOTHING = -1;
    static const unsigned long NAME_MAXLEN = 16;

    SendQueue();
    virtual ~SendQueue();

    ResultEnum Send(char* const targetName, void* const bufferPtr, const unsigned long size);
    ResultEnum TimedSend(char* const targetName, void* const bufferPtr, const unsigned long size, const long timeout);
    int GetLastError();

private:

    int m_LastError;

};

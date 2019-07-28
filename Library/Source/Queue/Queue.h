#pragma once

#include <mqueue.h>
#include "Include/Common.h"

class Queue
{
public:

    static const int INVALID_QUEUE = -1;
    static const int ERROR_NOTHING = -1;
    static const unsigned long NAME_MAXLEN = 16;

    Queue(char* const name);
    virtual ~Queue();

    ResultEnum Open();
    ResultEnum Close();
    ResultEnum IsSendable(bool& sendable);
    ResultEnum IsReceivable(bool& receivable);
    ResultEnum Send(char* const targetName, void* const bufferPtr, const unsigned long size);
    ResultEnum Receive(void* const bufferPtr, const unsigned long size);
    int GetLastError();

private:

    mqd_t m_Queue;
    char m_Name[NAME_MAXLEN];
    int m_LastError;

};

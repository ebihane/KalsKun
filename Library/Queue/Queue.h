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
    ResultEnum Send(char* const targetName, void* const bufferPtr, const unsigned long size);
    ResultEnum IsReceivable(bool& receivable);
    ResultEnum Receive(void* const bufferPtr);
    int GetLastError();

private:

    mqd_t m_Queue;
    mq_attr m_Attr;
    char m_Name[NAME_MAXLEN];
    int m_LastError;

};

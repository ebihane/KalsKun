#pragma once

#include <mqueue.h>
#include "Adapter/AdapterBase.h"

class Queue : public AdapterBase
{
public:

    static const int INVALID_QUEUE = -1;
    static const unsigned long NAME_MAXLEN = 16;

    Queue(char* const name);
    virtual ~Queue();

    ResultEnum Open();
    ResultEnum Close();
    ResultEnum Connection();
    ResultEnum Disconnection();

    ResultEnum IsSendable(bool& sendable);
    ResultEnum Send(void* const bufferPtr, const unsigned long size);
    ResultEnum Send(char* const targetName, void* const bufferPtr, const unsigned long size);
    ResultEnum IsReceivable(bool& receivable);
    ResultEnum Receive(void* const bufferPtr);
    ResultEnum Receive(void* const bufferPtr, const unsigned long size);

private:

    mqd_t m_Queue;
    mq_attr m_Attr;
    char m_Name[NAME_MAXLEN];

};

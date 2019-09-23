#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <errno.h>
#include "SendQueue.h"

//
// notice : コンパイルオプションに rt 指定
//

SendQueue::SendQueue()
 : m_LastError(ERROR_NOTHING)
{
    /* nop. */
}

SendQueue::~SendQueue()
{
    /* nop. */
}

ResultEnum SendQueue::Send(char* const targetName, void* const bufferPtr, const unsigned long size)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    mqd_t targetQueue = INVALID_QUEUE;
    char name[NAME_MAXLEN + 1] = { 0 };

    if ((targetName == NULL) || (bufferPtr == NULL))
    {
        goto FINISH;
    }

    if (targetName[0] != '/')
    {
        snprintf(&name[0], sizeof(name), "/%s", &targetName[0]);
    }
    else
    {
        strncpy(&name[0], &targetName[0], NAME_MAXLEN);
    }

    m_LastError = ERROR_NOTHING;

    /* 送信対象のキューをオープン */
    targetQueue = mq_open(&name[0], O_WRONLY);
    if (targetQueue < 0)
    {
        printf("[SendQueue::Send] mq_open failed. target[%s]\n", name);
        m_LastError = errno;
        goto FINISH;
    }

    if (mq_send(targetQueue, (const char*)bufferPtr, size, 0) < 0)
    {
        printf("[SendQueue::Send] mq_send failed. target[%s]\n", targetName);
        m_LastError = errno;
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;
    

FINISH :
    return retVal;
}

ResultEnum SendQueue::TimedSend(char* const targetName, void* const bufferPtr, const unsigned long size, const long timeoutSec)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    mqd_t targetQueue = INVALID_QUEUE;
    char name[NAME_MAXLEN + 1] = { 0 };
    struct timespec timeout = { 0 };

    if ((targetName == NULL) || (bufferPtr == NULL))
    {
        goto FINISH;
    }

    if (targetName[0] != '/')
    {
        snprintf(&name[0], sizeof(name), "/%s", &targetName[0]);
    }
    else
    {
        strncpy(&name[0], &targetName[0], NAME_MAXLEN);
    }

    m_LastError = ERROR_NOTHING;

    /* 送信対象のキューをオープン */
    targetQueue = mq_open(&name[0], O_WRONLY);
    if (targetQueue < 0)
    {
        printf("[SendQueue::Send] mq_open failed. target[%s]\n", name);
        m_LastError = errno;
        goto FINISH;
    }

    if (timeoutSec == 0)
    {
        /* 1ms */
        timeout.tv_nsec = 1000000;
        timeout.tv_nsec = 0;
    }
    else
    {
        timeout.tv_sec = timeoutSec;
        timeout.tv_nsec = 0;
    }

    if (mq_timedsend(targetQueue, (const char*)bufferPtr, size, 0, &timeout) < 0)
    {
        printf("[SendQueue::Send] mq_send failed. target[%s]\n", targetName);
        m_LastError = errno;
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;


FINISH:
    return retVal;
}

int SendQueue::GetLastError()
{
    return m_LastError;
}

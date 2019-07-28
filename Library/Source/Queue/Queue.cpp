#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>
#include "Queue.h"

//
// notice : コンパイルオプションに rt 指定
//

Queue::Queue(char* const name)
 : m_Queue(INVALID_QUEUE)
 , m_LastError(ERROR_NOTHING)
{
    snprintf(&m_Name[0], NAME_MAXLEN, "/%s", &name[0]);
}

Queue::~Queue()
{
    Close();
}

ResultEnum Queue::Open()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    int errorNo = ERROR_NOTHING;
    mqd_t queue = 0;

    /* 存在確認 */
    m_LastError = ERROR_NOTHING;
    queue = mq_open(&m_Name[0], O_RDWR);
    if (0 <= queue)
    {
        /* 存在している場合はそのキューのハンドルを取得 */
        m_Queue = queue;
        retVal = ResultEnum::NormalEnd;
        goto FINISH;
    }

    /* errno 保持 */
    errorNo = errno;

    /* 存在しない場合の errno 以外の異常が発生した場合 */
    if (errorNo != ENOENT)
    {
        m_LastError = errorNo;
        goto FINISH;
    }

    /* 存在しない場合は新規作成 */
    queue = mq_open(&m_Name[0], O_RDWR | O_CREAT, 0777, NULL);
    if (queue < 0)
    {
        m_LastError = errno;
        goto FINISH;
    }

    m_Queue = queue;

    retVal = ResultEnum::NormalEnd;


FINISH :
    return retVal;
}

ResultEnum Queue::Close()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    if (m_Queue != INVALID_QUEUE)
    {
        if (mq_close(m_Queue) < 0)
        {
            m_LastError = errno;
            goto FINISH;
        }
    }

    retVal = ResultEnum::NormalEnd;


FINISH :
    return retVal;
}

ResultEnum Queue::IsSendable(bool& sendable)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    fd_set sendFds;
    fd_set errorFds;
    struct timeval  timeout;

    sendable = false;

    if (m_Queue == INVALID_QUEUE)
    {
        goto FINISH;
    }

    FD_ZERO(&sendFds);
    FD_ZERO(&errorFds);
    FD_SET(m_Queue, &sendFds);
    FD_SET(m_Queue, &errorFds);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    m_LastError = ERROR_NOTHING;
    if (select(m_Queue + 1, NULL, &sendFds, &errorFds, &timeout) < 0)
    {
        m_LastError = errno;
    }
    else if (FD_ISSET(m_Queue, &errorFds) != 0)
    {
        m_LastError = errno;
    }
    else if (FD_ISSET(m_Queue, &sendFds) == 0)
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

ResultEnum Queue::IsReceivable(bool& receivable)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    fd_set recvFds;
    fd_set errorFds;
    struct timeval timeout;

    receivable = false;

    if (m_Queue == INVALID_QUEUE)
    {
        goto FINISH;
    }

    FD_ZERO(&recvFds);
    FD_ZERO(&errorFds);
    FD_SET(m_Queue, &recvFds);
    FD_SET(m_Queue, &errorFds);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    m_LastError = ERROR_NOTHING;
    if (select(m_Queue + 1, &recvFds, NULL, &errorFds, &timeout) < 0)
    {
        m_LastError = errno;
    }
    else if (FD_ISSET(m_Queue, &errorFds) != 0)
    {
        m_LastError = errno;
    }
    else if (FD_ISSET(m_Queue, &recvFds) == 0)
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

ResultEnum Queue::Send(char* const targetName, void* const bufferPtr, const unsigned long size)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    mqd_t targetQueue = INVALID_QUEUE;


    if ((targetName == NULL) || (bufferPtr == NULL))
    {
        goto FINISH;
    }

    m_LastError = ERROR_NOTHING;

    /* 送信対象のキューをオープン */
    targetQueue = mq_open(&targetName[0], O_RDWR);
    if (targetQueue < 0)
    {
        m_LastError = errno;
        goto FINISH;
    }

    if (mq_send(targetQueue, (const char*)bufferPtr, size, 0) < 0)
    {
        m_LastError = errno;
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;
    

FINISH :
    return retVal;
}

ResultEnum Queue::Receive(void* const bufferPtr, const unsigned long size)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;


    if (m_Queue == INVALID_QUEUE)
    {
        goto FINISH;
    }

    m_LastError = ERROR_NOTHING;
    if (mq_receive(m_Queue, (char *)bufferPtr, size, 0) < 0)
    {
        m_LastError = errno;
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;


FINISH:
    return retVal;
}

int Queue::GetLastError()
{
    return m_LastError;
}

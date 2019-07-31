#define _OPEN_THREADS

#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <wiringPi.h>
#include "ThreadBase.h"

//
// notice : コンパイルオプションに pthread 指定
//

ThreadBase::ThreadBase()
 : m_Handle(-1)
 , m_Stop(false)
 , m_LastError(ERROR_NOTHING)
 , m_ThreadState(ThreadStateEnum::NotStart)
 , m_ThreadResult(ResultEnum::AbnormalEnd)
{
    /* nop. */
}

ThreadBase::~ThreadBase()
{
    Stop(10);
}

ResultEnum ThreadBase::Run()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    pthread_t tid = -1;
    pthread_attr_t  attr;


    m_Stop = false;
    m_ThreadState = ThreadStateEnum::NotStart;
    m_LastError = ERROR_NOTHING;

    if (pthread_attr_init(&attr) != 0)
    {
        m_LastError = errno;
        m_ThreadState = ThreadStateEnum::Error;
        goto FINISH;
    }

    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
    {
        m_LastError = errno;
        m_ThreadState = ThreadStateEnum::Error;
        goto FINISH;
    }

    if (initialize() != ResultEnum::NormalEnd)
    {
        m_ThreadState = ThreadStateEnum::Error;
        goto FINISH;
    }

    if (pthread_create(&tid, &attr, this->threadMain, this) != 0)
    {
        m_LastError = errno;
        m_ThreadState = ThreadStateEnum::Error;
        goto FINISH;
    }

    m_Handle = (int)tid;

    retVal = ResultEnum::NormalEnd;


FINISH :
    return retVal;
}

ResultEnum ThreadBase::Stop(const unsigned long timeoutSec)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    unsigned long cnt = 0;
    unsigned long timeoutSec100msec = timeoutSec * 10;

    m_Stop = true;
    while (1)
    {
        if (timeoutSec100msec < cnt)
        {
            goto FINISH;
        }

        if (m_ThreadState != ThreadStateEnum::Executing)
        {
            break;
        }

        cnt++;
        delay(100);
    }

    retVal = ResultEnum::NormalEnd;


FINISH :
    return retVal;
}

void ThreadBase::MainProcedure()
{
    m_ThreadState = ThreadStateEnum::Executing;
    m_ThreadResult = ResultEnum::AbnormalEnd;

    m_ThreadResult = doProcedure();
    finalize();

    m_ThreadState = ThreadStateEnum::Finished;
}

ThreadBase::ThreadStateEnum ThreadBase::GetState()
{
    return m_ThreadState;
}

int ThreadBase::GetLastError()
{
    return m_LastError;
}

bool ThreadBase::isStopRequest()
{
    return m_Stop;
}

void* ThreadBase::threadMain(void* param)
{
    void* retVal = NULL;

    ThreadBase* p = (ThreadBase *)param;
    p->MainProcedure();

    return retVal;
}

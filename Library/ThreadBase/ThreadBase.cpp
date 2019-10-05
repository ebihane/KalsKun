#define _OPEN_THREADS

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <wiringPi.h>
#include <exception>
#include "Measure/Stopwatch.h"
#include "ThreadBase.h"

//
// notice : コンパイルオプションに pthread 指定
//

ThreadBase::ThreadBase(char* const taskName)
 : m_Logger(NULL)
 , m_Handle(-1)
 , m_Stop(false)
 , m_LastError(ERROR_NOTHING)
 , m_ThreadState(ThreadStateEnum::NotStart)
 , m_ThreadResult(ResultEnum::AbnormalEnd)
{
    memset(&m_TaskName[0], '\0', sizeof(m_TaskName));
    strncpy(&m_TaskName[0], &taskName[0], sizeof(m_TaskName));
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

    m_Logger = new Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::BOTH_OUT);
    if (m_Logger == NULL)
    {
        m_LastError = errno;
        m_ThreadState = ThreadStateEnum::Error;
        goto FINISH;
    }

    if (pthread_attr_init(&attr) != 0)
    {
        m_LastError = errno;
        m_ThreadState = ThreadStateEnum::Error;
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::ThreadBase] pthread_attr_init failed.\n", m_TaskName);
        m_Logger->LOG_ERROR(m_LogStr);
        goto FINISH;
    }

    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
    {
        m_LastError = errno;
        m_ThreadState = ThreadStateEnum::Error;
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::ThreadBase] pthread_attr_setdetachstate failed.\n", m_TaskName);
        m_Logger->LOG_ERROR(m_LogStr);
        goto FINISH;
    }

    if (initialize() != ResultEnum::NormalEnd)
    {
        m_ThreadState = ThreadStateEnum::Error;
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::ThreadBase] initialize failed.\n", m_TaskName);
        m_Logger->LOG_ERROR(m_LogStr);
        goto FINISH;
    }

    if (pthread_create(&tid, &attr, this->threadMain, this) != 0)
    {
        m_LastError = errno;
        m_ThreadState = ThreadStateEnum::Error;
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::ThreadBase] pthread_create failed.\n", m_TaskName);
        m_Logger->LOG_ERROR(m_LogStr);
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
    Stopwatch watch;

    m_Stop = true;
    snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::Stop] Stop Request set. timeout[%ld sec]\n", m_TaskName, timeoutSec);
    m_Logger->LOG_INFO(m_LogStr);

    watch.Start();
    while (1)
    {
        if ((float)timeoutSec <= watch.GetSplit())
        {
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::Stop] Stop Timeout.\n", m_TaskName);
            m_Logger->LOG_ERROR(m_LogStr);
            goto FINISH;
        }

        if (m_ThreadState != ThreadStateEnum::Executing)
        {
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::Stop] Thread stopped.\n", m_TaskName);
            m_Logger->LOG_INFO(m_LogStr);
            break;
        }

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

    try
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s] Thread start.\n", &m_TaskName[0]);
        m_Logger->LOG_INFO(m_LogStr);

        m_ThreadResult = doProcedure();
    }
    catch (std::exception& e)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s] Exception! [%s]\n", &m_TaskName[0], e.what());
        m_Logger->LOG_ERROR(m_LogStr);
    }

    finalize();

    snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s] Thread finish.\n", &m_TaskName[0]);
    m_Logger->LOG_INFO(m_LogStr);

    if (m_Logger != NULL)
    {
        delete m_Logger;
        m_Logger = NULL;
    }

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

void ThreadBase::ChangeLogInfo(const char logLevel, const Logger::LogTypeEnum logType)
{
    if (m_Logger != NULL)
    {
        m_Logger->ChangeLevel(logLevel);
        m_Logger->ChangeOut(logType);
    }
}

void ThreadBase::GetName(char* const name)
{
    strncpy(&name[0], &m_TaskName[0], sizeof(m_TaskName));
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

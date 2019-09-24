#pragma once

#include "Include/Common.h"
#include "Logger/Logger.h"

class ThreadBase
{
public :

    static const int ERROR_NOTHING = -1;

    typedef enum
    {
        NotStart,
        Executing,
        Finished,
        Error,
    } ThreadStateEnum;

    ThreadBase(char* const taskName);
    virtual ~ThreadBase();

    ResultEnum Run();
    ResultEnum Stop(const unsigned long timeoutSec);
    void MainProcedure();
    ThreadStateEnum GetState();
    int GetLastError();
    void ChangeLogInfo(const char logLevel, const Logger::LogTypeEnum logType);

protected :

    Logger* m_Logger;
    char m_TaskName[16];

    virtual ResultEnum initialize() = 0;
    virtual ResultEnum doProcedure() = 0;
    virtual ResultEnum finalize() = 0;
    bool isStopRequest();

private :

    int m_Handle;
    bool m_Stop;
    int m_LastError;
    ThreadStateEnum m_ThreadState;
    ResultEnum m_ThreadResult;

    static void* threadMain(void* param);
};
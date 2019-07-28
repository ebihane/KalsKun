#pragma once
#include <stdio.h>
#include <time.h>
#include "Queue/Queue.h"
#include "ThreadBase/ThreadBase.h"

class LogWriter : public ThreadBase
{
public :

    static const long CONTROLLER_NAME_LENGTH = 16;

    LogWriter(char* const name);
    virtual ~LogWriter();

protected :

private :

    static const long LOG_LENGTH_MAX = 128;

    typedef struct
    {
        long Code;
        char Buffer[LOG_LENGTH_MAX];
    } LogMessageStr;

    time_t m_FileTime;
    FILE* m_FilePtr;
    Queue* m_Queue;
    LogMessageStr* m_ReceiveBuffer = NULL;
    char m_ControllerName[CONTROLLER_NAME_LENGTH];

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

    void doLogWrite(char* const pMessage);
    void doLogStop();
    void doLogRestart();

    bool isDateChange();
    void fileReopen();

};

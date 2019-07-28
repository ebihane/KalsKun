#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "LogWriter.h"

LogWriter::LogWriter(char* const name)
 : ThreadBase()
 , m_FileTime(-1)
 , m_FilePtr(NULL)
 , m_Queue(NULL)
 , m_ReceiveBuffer(NULL)
{
    strncpy(&m_ControllerName[0], name, CONTROLLER_NAME_LENGTH);
}

LogWriter::~LogWriter()
{
    /* nop. */
}

ResultEnum LogWriter::initialize()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    LogMessageStr* pBuffer = NULL;
    Queue* pQueue = NULL;

    if (m_ReceiveBuffer == NULL)
    {
        pBuffer = new LogMessageStr;
        if (pBuffer == NULL)
        {
            goto FINISH;
        }

        m_ReceiveBuffer = pBuffer;
    }

    if (m_Queue == NULL)
    {
        pQueue = new Queue((char *)"LogWriter");
        if (pQueue == NULL)
        {
            goto FINISH;
        }

        if (pQueue->Open() != ResultEnum::NormalEnd)
        {
            goto FINISH;
        }

        m_Queue = pQueue;
    }

    fileReopen();

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

ResultEnum LogWriter::doProcedure()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    bool receivable = false;

    if (m_Queue == NULL)
    {
        goto FINISH;
    }

    while (1)
    {
        receivable = false;
        if (m_Queue->IsReceivable(receivable) < 0)
        {
            goto FINISH;
        }

        if (receivable == false)
        {
            if (isStopRequest() == true)
            {
                break;
            }

            if (isDateChange() == true)
            {
                fileReopen();
            }

            delay(10);
            continue;
        }

        /* 受信 */
        m_Queue->Receive(&m_ReceiveBuffer[0], sizeof(m_ReceiveBuffer));

        /* イベント分岐 */
        switch (m_ReceiveBuffer->Code)
        {
            case LOG_EVCODE_OUTPUT :
                doLogWrite(&m_ReceiveBuffer->Buffer[0]);
                break;

            case LOG_EVCODE_STOP :
                doLogStop();
                break;

            case LOG_EVCODE_RESTART :
                doLogRestart();
                break;

            default :
                break;
        }
    }

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

ResultEnum LogWriter::finalize()
{
    doLogStop();

    if (m_FilePtr != NULL)
    {
        fclose(m_FilePtr);
        m_FilePtr = NULL;
    }

    if (m_ReceiveBuffer != NULL)
    {
        delete m_ReceiveBuffer;
        m_ReceiveBuffer = NULL;
    }

    if (m_Queue != NULL)
    {
        delete m_Queue;
        m_Queue = NULL;
    }

    return ResultEnum::NormalEnd;
}

void LogWriter::doLogWrite(char* const pMessage)
{
    if (m_FilePtr != NULL)
    {
        fprintf(m_FilePtr, pMessage);
    }
}

void LogWriter::doLogStop()
{
    if (m_FilePtr != NULL)
    {
        fflush(m_FilePtr);
        fclose(m_FilePtr);
        m_FilePtr = NULL;
    }
}

void LogWriter::doLogRestart()
{
    fileReopen();
}

bool LogWriter::isDateChange()
{
    bool retVal = false;
    time_t t = time(NULL);
    struct tm* tmPtr = localtime(&t);
    

    if (m_FileTime < 0)
    {
        retVal = true;
    }
    else
    {
        struct tm* nowTmPtr = localtime(&m_FileTime);

        if ((tmPtr->tm_year == nowTmPtr->tm_year)
        &&  (tmPtr->tm_mon == nowTmPtr->tm_mon)
        &&  (tmPtr->tm_mday == nowTmPtr->tm_mday))
        {
            retVal = false;
        }
        else
        {
            retVal = true;
        }
    }

    return retVal;
}

void LogWriter::fileReopen()
{
    if (m_FilePtr != NULL)
    {
        fclose(m_FilePtr);
        m_FilePtr = NULL;
    }

    time_t nowTime = time(NULL);
    struct tm* tmPtr = localtime(&nowTime);

    char folderPath[32] = {0};
    snprintf(&folderPath[0], sizeof(folderPath), "/home/pi/%s", &m_ControllerName[0]);
        
    char logPath[40] = {0};
    snprintf(&logPath[0], sizeof(logPath), "%s/20%02d_%02d_%02d.log", &folderPath[0], tmPtr->tm_year - 100, tmPtr->tm_mon, tmPtr->tm_mday);
    
    mkdir(&folderPath[0], 0777);
    FILE* fp = fopen(&logPath[0], "a");
    if (fp != NULL)
    {
        m_FilePtr = fp;
        m_FileTime = nowTime;
    }
}

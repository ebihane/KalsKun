#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "Logger.h"

Logger::Logger(char* const name, const long logLevel, const LogTypeEnum logType)
 : m_Queue(NULL)
 , m_Level(logLevel)
 , m_Type(logType)
{
    m_Queue = new Queue(name);
}

Logger::~Logger()
{
    if (m_Queue != NULL)
    {
        delete m_Queue;
        m_Queue = NULL;
    }
}

void Logger::ChangeLevel(const char logLevel)
{
    m_Level = logLevel;
}

void Logger::ChangeOut(const LogTypeEnum logType)
{
    m_Type = logType;
}

void Logger::Print(char* const buffer, const char logLevel, char* const fileName, const long lineNo)
{
    LogMessageStr* pMessage = NULL;
    struct timeval tmVal;
    struct tm* tmPtr = NULL;

    if (buffer == NULL)
    {
        goto FINISH;
    }

    if ((logLevel & m_Level) == 0)
    {
        goto FINISH;
    }

    pMessage = new LogMessageStr;
    if (pMessage == NULL)
    {
        goto FINISH;
    }

    gettimeofday(&tmVal, 0);
    tmPtr = localtime(&tmVal.tv_sec);

    pMessage->Code = LOG_OUTPUT;

    memset(&pMessage->Buffer[0], 0x00, LOG_LENGTH_MAX);
    snprintf(&pMessage->Buffer[0], LOG_LENGTH_MAX, "[%02d:%02d:%02d.%03d] <%s>-L%ld: ",
        tmPtr->tm_hour, tmPtr->tm_min, tmPtr->tm_sec, tmVal.tv_usec / 1000, basename(fileName), lineNo);

    strncat(&pMessage->Buffer[0], buffer, LOG_OUT_MAX);

    if (strchr(buffer, '\n') == NULL)
    {
        strncat(&pMessage->Buffer[0], "\n", LOG_LENGTH_MAX);
    }

    if ((m_Type == DISPLAY_OUT) || (m_Type == BOTH_OUT))
    {
        printf(pMessage->Buffer);
    }

    if ((m_Type == FILE_OUT) || (m_Type == BOTH_OUT))
    {
        if (m_Queue->Send((char*)"LogWriter", pMessage, sizeof(LogMessageStr)) != ResultEnum::NormalEnd)
        {
            printf("[Logger] Queue Send Failed. errno[%d]\n", m_Queue->GetLastError());
        }
    }

FINISH :

    if (pMessage != NULL)
    {
        delete pMessage;
        pMessage = NULL;
    }

    return;
}

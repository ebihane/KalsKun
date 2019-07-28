#pragma once
#include "Queue/Queue.h"

#define LOG_LENGTH_MAX  (128)
#define LOG_OUT_MAX     (80)

class Logger
{
public :

    static const long LOG_OUTPUT = 1;

    static const char LOG_ERROR = 0x01;
    static const char LOG_INFO = 0x02;
    static const char LOG_DEBUG = 0x04;

    typedef enum
    {
        FILE_OUT = 0x01,
        DISPLAY_OUT = 0x02,
        BOTH_OUT = 0x03,
    } LogTypeEnum;

    Logger(char* const name, const long logLevel, const LogTypeEnum logType);
    virtual ~Logger();

    void ChangeLevel(const char logLevel);
    void ChangeOut(const LogTypeEnum logType);

    void Print(char* const buffer, const char logLevel, char* const fileName, const long lineNo);

protected :

private :

    typedef struct
    {
        long Code;
        char Buffer[LOG_LENGTH_MAX];
    } LogMessageStr;

    Queue* m_Queue;
    long m_Level;
    LogTypeEnum m_Type;

};

#define LOG_ERROR(buffer)   Print((char *)buffer, Logger::LOG_ERROR, (char *)__FILE__, __LINE__)
#define LOG_INFO(buffer)    Print((char *)buffer, Logger::LOG_INFO, (char *)__FILE__, __LINE__)
#define LOG_DEBUG(buffer)   Print((char *)buffer, Logger::LOG_DEBUG, (char *)__FILE__, __LINE__)

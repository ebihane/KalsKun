#pragma once
#include "Queue/SendQueue.h"

#define LOG_LENGTH_MAX  (128)
#define LOG_OUT_MAX     (80)

/* ログ出力プロセス起動 */
int StartLoggerProcess(char* const name);

/* ログ出力プロセス停止 */
void StopLoggerProcess();

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

    Logger(const long logLevel, const LogTypeEnum logType);
    virtual ~Logger();

    void ChangeLevel(const char logLevel);
    void ChangeOut(const LogTypeEnum logType);

    void Print(char* const buffer, const char logLevel, char* const fileName, const long lineNo);

protected :

private :

    SendQueue m_SendQueue;
    long m_Level;
    LogTypeEnum m_Type;

};

#define LOG_ERROR(buffer)   Print((char *)buffer, Logger::LOG_ERROR, (char *)__FILE__, __LINE__)
#define LOG_INFO(buffer)    Print((char *)buffer, Logger::LOG_INFO, (char *)__FILE__, __LINE__)
#define LOG_DEBUG(buffer)   Print((char *)buffer, Logger::LOG_DEBUG, (char *)__FILE__, __LINE__)

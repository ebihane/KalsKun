#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include "Queue/Queue.h"

#define NAME_MAXLEN     (16)
#define LOG_LENGTH_MAX  (128)

typedef struct
{
    long Code;
    char Buffer[LOG_LENGTH_MAX];
} LogMessageStr;

static char g_cControllerName[NAME_MAXLEN + 1] = { 0 };
static time_t m_FileTime = -1;
static Queue* g_pQueue = NULL;
static FILE* g_pFilePtr = NULL;
static LogMessageStr* g_pLogMessageStrPtr = NULL;

bool initialize();
void terminate();
void doLogWrite(char* const pMessage);
void doLogStop();
void doLogRestart();
bool isDateChange();
void fileReopen();
void signalHandler(int signum);

int main(int argc, char* argv[])
{
    printf("[LogWriter]::Enter.\n");

    bool receivable = false;

    if (argc <= 1)
    {
        strncpy(&g_cControllerName[0], "Unknown", NAME_MAXLEN);
    }
    else
    {
        strncpy(&g_cControllerName[0], &argv[1][0], NAME_MAXLEN);
    }

    signal(SIGKILL, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGABRT, signalHandler);

    if (initialize() != true)
    {
        printf("[LogWriter] initialize failed.\n");
        goto FINISH;
    }

    printf("[LogWriter] Main loop start.\n");
    while (1)
    {
        receivable = false;
        if (g_pQueue->IsReceivable(receivable) < 0)
        {
            {
                char logStr[60] = { 0 };
                snprintf(&logStr[0], sizeof(logStr), "ERR! [LogWriter] Queue::IsReceivable failed. errno[%d]\n", g_pQueue->GetLastError());
                doLogWrite(&logStr[0]);
            }
            goto FINISH;
        }

        if (receivable == false)
        {
            if (isDateChange() == true)
            {
                fileReopen();
            }

            delay(10);
            continue;
        }

        /* 受信 */
        g_pQueue->Receive(&g_pLogMessageStrPtr[0]);

        /* イベント分岐 */
        switch (g_pLogMessageStrPtr->Code)
        {
        case LOG_EVCODE_OUTPUT:
            doLogWrite(&(g_pLogMessageStrPtr->Buffer[0]));
            break;

        case LOG_EVCODE_STOP:
            doLogStop();
            break;

        case LOG_EVCODE_RESTART:
            doLogRestart();
            break;

        case LOG_EVCODE_FINISH :
            goto FINISH;

        default:
            break;
        }
    }

FINISH :

    terminate();

    printf("[LogWriter]::Exit.\n");
	return 0;
}

bool initialize()
{
    bool retVal = false;
    LogMessageStr* pBuffer = NULL;
    Queue* pQueue = NULL;

    wiringPiSetupSys();

    if (g_pLogMessageStrPtr == NULL)
    {
        pBuffer = new LogMessageStr;
        if (pBuffer == NULL)
        {
            goto FINISH;
        }

        g_pLogMessageStrPtr = pBuffer;
    }

    if (g_pQueue == NULL)
    {
        pQueue = new Queue((char*)LOG_WRITER_QUEUE_NAME);
        if (pQueue == NULL)
        {
            goto FINISH;
        }

        if (pQueue->Open() != ResultEnum::NormalEnd)
        {
            goto FINISH;
        }

        g_pQueue = pQueue;
    }

    fileReopen();

    retVal = true;

FINISH:
    return retVal;
}

void terminate()
{
    doLogStop();

    if (g_pFilePtr != NULL)
    {
        fclose(g_pFilePtr);
        g_pFilePtr = NULL;
    }

    if (g_pLogMessageStrPtr != NULL)
    {
        delete g_pLogMessageStrPtr;
        g_pLogMessageStrPtr = NULL;
    }

    if (g_pQueue != NULL)
    {
        g_pQueue->Close();
        delete g_pQueue;
        g_pQueue = NULL;
    }
}

void doLogWrite(char* const pMessage)
{
    printf(pMessage);

    if (g_pFilePtr != NULL)
    {
        fprintf(g_pFilePtr, pMessage);
        fflush(g_pFilePtr);
    }
}

void doLogStop()
{
    doLogWrite((char *)"[LogWriter] Log Stop.\n");

    if (g_pFilePtr != NULL)
    {
        fflush(g_pFilePtr);
        fclose(g_pFilePtr);
        g_pFilePtr = NULL;
    }
}

void doLogRestart()
{
    fileReopen();
}

bool isDateChange()
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

void fileReopen()
{
    doLogWrite((char *)"[LogWriter] Log Reopen.\n");

    if (g_pFilePtr != NULL)
    {
        fclose(g_pFilePtr);
        g_pFilePtr = NULL;
    }

    time_t nowTime = time(NULL);
    struct tm* tmPtr = localtime(&nowTime);

    char folderPath[32] = { 0 };
    snprintf(&folderPath[0], sizeof(folderPath), "/home/pi/%s", &g_cControllerName[0]);

    char logPath[40] = { 0 };
    snprintf(&logPath[0], sizeof(logPath), "%s/20%02d_%02d_%02d.log", &folderPath[0], tmPtr->tm_year - 100, tmPtr->tm_mon + 1, tmPtr->tm_mday);

    mkdir(&folderPath[0], 0777);
    FILE* fp = fopen(&logPath[0], "a");
    if (fp == NULL)
    {
        printf("ERR! [LogWriter] Reopen failed. errno[%d]\n", errno);
        goto FINISH;
    }

    g_pFilePtr = fp;
    m_FileTime = nowTime;

FINISH :
    return;
}

void signalHandler(int signum)
{
    printf("ERR! [LogWriter] Signal Catched. signum[%d]\n", signum);

    {
        char logStr[60] = { 0 };
        snprintf(&logStr[0], sizeof(logStr), "[LogWriter] Signal Catched. signum[%d]\n", signum);
        doLogWrite(&logStr[0]);
    }

    terminate();

    exit(0);
}

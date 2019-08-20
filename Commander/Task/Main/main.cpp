#define MEMORY_MAIN

#include <stdlib.h>
#include "Logger/Logger.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Task/AroundCamera/AroundCameraReceiver.h"

static Logger* g_pLogger = NULL;
static AroundCameraReceiver* g_pAroundCameraReceiver = NULL;

ResultEnum initialize();
void mainProcedure();
void finalize();

int main(void)
{
    if (initialize() != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    mainProcedure();

FINISH :
    finalize();
    return 0;
}

ResultEnum initialize()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    wiringPiSetupSys();

    pShareMemory = new ShareMemoryStr();
    if (pShareMemory == NULL)
    {
        goto FINISH;
    }

    StartLoggerProcess((char*)"Commander");

    g_pLogger = new Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::LogTypeEnum::BOTH_OUT);
    if (g_pLogger == NULL)
    {
        goto FINISH;
    }

    g_pAroundCameraReceiver = new AroundCameraReceiver();
    if (g_pAroundCameraReceiver != NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] FrontCameraSender allocation failed.\n");
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

void mainProcedure()
{
    while (1)
    {
        delay(10);
    }
}

void finalize()
{
    if (g_pAroundCameraReceiver != NULL)
    {
        g_pAroundCameraReceiver->Stop(10000);
        delete g_pAroundCameraReceiver;
        g_pAroundCameraReceiver = NULL;
    }

    if (g_pLogger != NULL)
    {
        delete g_pLogger;
        g_pLogger = NULL;
    }
}
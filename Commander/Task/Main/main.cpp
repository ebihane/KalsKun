#include <stdlib.h>
#include "Logger/Logger.h"
#include "Task/AroundCamera/AroundCameraComm.h"

static Logger* g_pLogger = NULL;
static AroundCameraComm* g_pAroundCameraComm = NULL;

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

    StartLoggerProcess((char*)"Commander");

    g_pLogger = new Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::LogTypeEnum::BOTH_OUT);
    if (g_pLogger == NULL)
    {
        goto FINISH;
    }

    g_pAroundCameraComm = new AroundCameraComm();
    if (g_pAroundCameraComm != NULL)
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
    if (g_pAroundCameraComm != NULL)
    {
        g_pAroundCameraComm->Stop(10000);
        delete g_pAroundCameraComm;
        g_pAroundCameraComm = NULL;
    }

    if (g_pLogger != NULL)
    {
        delete g_pLogger;
        g_pLogger = NULL;
    }
}
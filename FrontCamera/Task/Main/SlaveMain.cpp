#include <stdlib.h>
#include "Include/Common.h"
#include "Logger/Logger.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Task/CameraCapture/CameraCapture.h"
#include "Task/CameraSender/CameraSender.h"
#include "SlaveMain.h"

static Logger* g_pLogger = NULL;
static CameraCapture* g_pCameraCapture = NULL;
static CameraSender* g_pCameraSender = NULL;

ResultEnum slaveInitialize();
void slaveFinalize();

ResultEnum slaveInitialize(const int cameraNo)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    g_pLogger = new Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::LogTypeEnum::BOTH_OUT);
    if (g_pLogger == NULL)
    {
        goto FINISH;
    }

    g_pCameraSender = new CameraSender();
    if (g_pCameraSender == NULL)
    {
        g_pLogger->LOG_ERROR("[slaveInitialize] g_pCameraSender allocation failed.\n");
        goto FINISH;
    }

    g_pCameraCapture = new CameraCapture(cameraNo);
    if (g_pCameraCapture == NULL)
    {
        g_pLogger->LOG_ERROR("[slaveInitialize] g_pCameraCapture allocation failed.\n");
        goto FINISH;
    }

    g_pCameraSender->Run();
    g_pCameraCapture->Run();

    retVal = ResultEnum::NormalEnd;

FINISH:
    return retVal;
}

void slaveFinalize()
{
    if (g_pCameraCapture != NULL)
    {
        g_pCameraCapture->Stop(5000);
        delete g_pCameraCapture;
        g_pCameraCapture = NULL;
    }

    if (g_pCameraSender != NULL)
    {
        delete g_pCameraSender;
        g_pCameraSender = NULL;
    }

    if (g_pLogger != NULL)
    {
        delete g_pLogger;
        g_pLogger = NULL;
    }
}

ResultEnum slaveMain(const int cameraNo)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    long captureIndex = -1;
    long sendIndex = -1;
    int key = 0;

    if (slaveInitialize(cameraNo) != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    g_pLogger->LOG_INFO("[slaveMain] Main loop start.\n");
    while (1)
    {
        if (g_pCameraCapture->IsCaptureStart() == true)
        {
            if (captureIndex != pShareMemory->Capture.Index)
            {
                captureIndex = pShareMemory->Capture.Index;
                cv::imshow("Capture", pShareMemory->Capture.Data[captureIndex]);
            }
        }

        if (sendIndex != pShareMemory->Communicate.Index)
        {
            sendIndex = pShareMemory->Communicate.Index;
            cv::imshow("Send", pShareMemory->Communicate.Data[sendIndex]);
        }

        key = cv::waitKey(1);
        if (key == 'q')
        {
            break;
        }
    }

    retVal = ResultEnum::NormalEnd;

FINISH :

    cv::destroyAllWindows();
    slaveFinalize();
    return retVal;
}

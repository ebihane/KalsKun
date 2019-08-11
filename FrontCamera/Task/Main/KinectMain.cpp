#define KINECT_CAPTURE_USE

#include <stdlib.h>
#include "Include/Common.h"
#include "Logger/Logger.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Task/Kinect/KinectCapture.h"
#include "KinectMain.h"

static Logger* g_pLogger = NULL;
static KinectCapture* g_pKinectCapture = NULL;

ResultEnum kinectInitialize(const int cameraNo);
void kinectFinalize();

ResultEnum kinectInitialize(const int cameraNo)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    g_pLogger = new Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::LogTypeEnum::BOTH_OUT);
    if (g_pLogger == NULL)
    {
        goto FINISH;
    }

    g_pKinectCapture = new KinectCapture();
    if (g_pKinectCapture == NULL)
    {
        g_pLogger->LOG_ERROR("[kinectInitialize] g_pKinectCapture allocation failed.\n");
        goto FINISH;
    }

    g_pKinectCapture->Start();

    retVal = ResultEnum::NormalEnd;

 FINISH :
    return retVal;
}

void kinectFinalize()
{
    if (g_pLogger != NULL)
    {
        delete g_pLogger;
        g_pLogger = NULL;
    }
}

ResultEnum kinectMain(const int cameraNo)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    unsigned char captureIndex = -1;
    int key = 0;

    if (kinectInitialize(cameraNo) != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    g_pLogger->LOG_INFO("[kinectMain] Main loop start.\n");
    while (1)
    {
        // ハートビート出力


        // 終了指示


        if (captureIndex != pShareMemory->Capture.Index)
        {
            captureIndex = pShareMemory->Capture.Index;
            cv::imshow("Capture", pShareMemory->Capture.Data[captureIndex]);
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
    kinectFinalize();
    return retVal;
}

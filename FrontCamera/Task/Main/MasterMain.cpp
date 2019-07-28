#include <stdlib.h>
#include "Include/Common.h"
#include "Logger/Logger.h"
#include "Socket/UdpReceiver/UdpReceiver.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Task/CameraCapture/CameraCapture.h"
#include "Task/CameraReceiver/CameraReceiver.h"
#include "MasterMain.h"

static Logger* g_pLogger = NULL;
static CameraCapture* g_pCameraCapture = NULL;
static CameraReceiver* g_pCameraReceiver = NULL;

ResultEnum masterInitialize();
void masterFinalize();

ResultEnum masterInitialize()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    g_pLogger = new Logger((char *)"Main", Logger::LOG_ERROR | Logger::LOG_INFO, Logger::LogTypeEnum::BOTH_OUT);
    if (g_pLogger == NULL)
    {
        goto FINISH;
    }

    g_pCameraReceiver = new CameraReceiver();
    if (g_pCameraReceiver == NULL)
    {
        g_pLogger->LOG_ERROR("[masterInitialize] g_pCameraReceiver allocation failed.\n");
        goto FINISH;
    }

    g_pCameraCapture = new CameraCapture(0);
    if (g_pCameraCapture == NULL)
    {
        g_pLogger->LOG_ERROR("[masterInitialize] g_pCameraCapture allocation failed.\n");
        goto FINISH;
    }

    g_pCameraReceiver->Run();
    g_pCameraCapture->Run();

    retVal = ResultEnum::NormalEnd;

 FINISH :
    return retVal;
}

void masterFinalize()
{
    if (g_pCameraCapture != NULL)
    {
        g_pCameraCapture->Stop(5000);
        delete g_pCameraCapture;
        g_pCameraCapture = NULL;
    }

    if (g_pCameraReceiver != NULL)
    {
        delete g_pCameraReceiver;
        g_pCameraReceiver = NULL;
    }

    if (g_pLogger != NULL)
    {
        delete g_pLogger;
        g_pLogger = NULL;
    }
}

ResultEnum masterMain()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    long captureIndex = -1;
    long receiveIndex = -1;
    int key = 0;

    if (masterInitialize() != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    g_pLogger->LOG_INFO("[masterMain] Main loop start.\n");
    while (1)
    {
#if 0
        // 受信データの更新を確認
        if (receiveIndex != pShareMemory->Communicate.Index)
        {
            receiveIndex = pShareMemory->Communicate.Index;
            Mat receiveCapture = pShareMemory->Communicate.Data[receiveIndex];
            cv::imshow("Receive", receiveCapture);

            // 受信データの更新がある場合のみ、Master の持つ Camera 画像を取り込む
            captureIndex = pShareMemory->Capture.Index;
            Mat masterCapture = pShareMemory->Capture.Data[captureIndex];
            cv::imshow("Capture", masterCapture);

            // 2つの画像からステレオマッチングを行い、物体との距離を測定する


        }

        // 画像更新無し時間の計測


        // ハートビート出力


        // 終了指示


#endif

        if (g_pCameraCapture->IsCaptureStart() == true)
        {
            if (captureIndex != pShareMemory->Capture.Index)
            {
                captureIndex = pShareMemory->Capture.Index;
                cv::imshow("Capture", pShareMemory->Capture.Data[captureIndex]);
            }
        }

        if (receiveIndex != pShareMemory->Communicate.Index)
        {
            receiveIndex = pShareMemory->Communicate.Index;
            cv::imshow("Receive", pShareMemory->Communicate.Data[receiveIndex]);
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
    masterFinalize();
    return retVal;
}

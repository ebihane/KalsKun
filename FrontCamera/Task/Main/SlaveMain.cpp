#include <stdlib.h>
#include <iostream>
#include <vector>
#include "Include/Common.h"
#include "Logger/Logger.h"
#include "Socket/TcpClient/TcpClient.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Task/CameraCapture/CameraCapture.h"
#include "Task/CameraSender/CameraSender.h"
#include "Task/StateSender/StateSender.h"
#include "Task/HeartBeat/HeartBeatManager.h"
#include "SlaveMain.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

static Logger* g_pLogger = NULL;
static CameraCapture* g_pCameraCapture = NULL;
static CameraSender* g_pCameraSender = NULL;
static StateSender* g_pStateSender = NULL;
static HeartBeatManager* g_pHeartBeatManager = NULL;

ResultEnum slaveInitialize(const int cameraNo);
void slaveFinalize();
bool isFaceExist(cv::Mat masterCapture, cv::CascadeClassifier cascade);

ResultEnum slaveInitialize(const int cameraNo)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    TcpClient* client = NULL;

    g_pLogger = new Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::LogTypeEnum::BOTH_OUT);
    if (g_pLogger == NULL)
    {
        goto FINISH;
    }

    /* ハートビート制御スレッド 初期化 */
    g_pHeartBeatManager = new HeartBeatManager();
    if (g_pHeartBeatManager == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] HeartBeatManager allocation failed.\n");
        goto FINISH;
    }
    g_pCameraCapture = new CameraCapture(cameraNo);
    if (g_pCameraCapture == NULL)
    {
        g_pLogger->LOG_ERROR("[slaveInitialize] g_pCameraCapture allocation failed.\n");
        goto FINISH;
    }

    client = new TcpClient("192.168.3.1", 10003);
    g_pStateSender = new StateSender(client);
    if (g_pStateSender == NULL)
    {
        g_pLogger->LOG_ERROR("[masterInitialize] g_pStateSender allocation failed.\n");
        goto FINISH;
    }

    g_pHeartBeatManager->Run();
    g_pCameraCapture->Run();
    g_pStateSender->Run();

    retVal = ResultEnum::NormalEnd;

FINISH:
    return retVal;
}

void slaveFinalize()
{
    if (g_pStateSender != NULL)
    {
        g_pStateSender->Stop(5000);
        delete g_pStateSender;
        g_pStateSender = NULL;
    }

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

    if (g_pHeartBeatManager != NULL)
    {
        g_pHeartBeatManager->Stop(5);
        delete g_pHeartBeatManager;
        g_pHeartBeatManager = NULL;
    }

    StopLoggerProcess();

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

    cv::Scalar				cvColor = cv::Scalar(0, 0, 256);
    cv::Point				cvPointStart = { 0x00 };

    cv::CascadeClassifier cascade;
    //	cascade = cv::CascadeClassifier("/home/pi/haarcascade_lowerbody.xml");
    //	cascade = cv::CascadeClassifier("/home/pi/haarcascade_frontalcatface.xml");
    cascade = cv::CascadeClassifier("/home/pi/haarcascade_frontalface_alt.xml");

    if (slaveInitialize(cameraNo) != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    g_pLogger->LOG_INFO("[slaveMain] Main loop start.\n");
    while (1)
    {
        captureIndex = pShareMemory->Capture.Index;
        Mat slaveCapture = pShareMemory->Capture.Data[captureIndex];
        if (slaveCapture.data != NULL)
        {
            bool faceDetected = isFaceExist(slaveCapture, cascade);
            if (faceDetected == true)
            {
                cvPointStart = Point(30, 130);
                cvColor = cv::Scalar(256, 256, 256);
                putText(slaveCapture, "face detected.", cvPointStart, FONT_HERSHEY_SIMPLEX, 1.2, cvColor, 2, cv::FONT_HERSHEY_DUPLEX);

                // 不審者検知
                pShareMemory->StateData = 1;
            }
            else
            {
                // 不審者非検知
                pShareMemory->StateData = 0;
            }

            cv::imshow("camera", slaveCapture);
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

bool isFaceExist(cv::Mat masterCapture, cv::CascadeClassifier cascade)
{
    bool retVal = false;

    cv::Mat gray_map;
    std::vector<cv::Rect> faces;
    cv::cvtColor(masterCapture, gray_map, cv::COLOR_BGR2GRAY);
    cv::Mat resizeMap;
    cv::Size size = cv::Size(320, 240);
    cv::resize(gray_map, resizeMap, size);
    cascade.detectMultiScale(resizeMap, faces, 1.1, 3, 0, cv::Size(20, 20));
    if (faces.size() > 0)
    {
        retVal = true;
    }

    //for (int i = 0; i < faces.size(); i++)
    //{
    //    rectangle(masterCapture, cv::Point(faces[i].x, faces[i].y), cv::Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), cv::Scalar(0, 0, 255), 3); //検出した顔を赤色矩形で囲む
    //    cv::putText(masterCapture, "DETECTION!!", cv::Point(faces[i].x + 30, faces[i].y), cv::FONT_HERSHEY_DUPLEX, 1.2, cv::Scalar(0, 0, 200), 2, 3);
    //    retVal = true;
    //}

    return retVal;
}

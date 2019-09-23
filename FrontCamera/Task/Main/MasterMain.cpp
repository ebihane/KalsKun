#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include "Include/Common.h"
#include "Parts/FrontCameraCommon.h"
#include "Logger/Logger.h"
#include "Socket/TcpClient/TcpClient.h"
#include "Socket/UdpReceiver/UdpReceiver.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Task/CameraCapture/CameraCapture.h"
#include "Task/CameraReceiver/CameraReceiver.h"
#include "Task/StateSender/StateSender.h"
#include "Task/HeartBeat/HeartBeatManager.h"
#include "Task/Ultrasound/UltrasoundManager.h"
#include "MasterMain.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

static Logger* g_pLogger = NULL;
static CameraCapture* g_pCameraCapture = NULL;
static CameraReceiver* g_pCameraReceiver = NULL;
static StateSender* g_pStateSender = NULL;
static HeartBeatManager* g_pHeartBeatManager = NULL;
static UltrasoundManager* g_pUltrasoundManager1 = NULL;
static UltrasoundManager* g_pUltrasoundManager2 = NULL;

ResultEnum masterInitialize(const int cameraNo);
void masterFinalize();
bool isColorLineDetected(cv::Mat masterCapture, cv::Scalar cvColor, long* parameters);

ResultEnum masterInitialize(const int cameraNo)
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

    /* 超音波1 制御スレッド 初期化 */
    g_pUltrasoundManager1 = new UltrasoundManager(0);
    if (g_pUltrasoundManager1 == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] UltrasoundManager allocation failed.\n");
        goto FINISH;
    }

    /* 超音波2 制御スレッド 初期化 */
    g_pUltrasoundManager2 = new UltrasoundManager(1);
    if (g_pUltrasoundManager2 == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] UltrasoundManager allocation failed.\n");
        goto FINISH;
    }

    /* 自身のカメラをキャプチャするスレッドの生成 */
    g_pCameraCapture = new CameraCapture(cameraNo);
    if (g_pCameraCapture == NULL)
    {
        g_pLogger->LOG_ERROR("[masterInitialize] g_pCameraCapture allocation failed.\n");
        goto FINISH;
    }

    client = new TcpClient("192.168.3.1", 10002);
    g_pStateSender = new StateSender(client);
    if (g_pStateSender == NULL)
    {
        g_pLogger->LOG_ERROR("[masterInitialize] g_pStateSender allocation failed.\n");
        goto FINISH;
    }

    g_pHeartBeatManager->Run();
    g_pCameraCapture->Run();
    g_pUltrasoundManager1->Run();
    g_pUltrasoundManager2->Run();
    g_pStateSender->Run();

    retVal = ResultEnum::NormalEnd;

 FINISH :
    return retVal;
}

void masterFinalize()
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

    if (g_pCameraReceiver != NULL)
    {
        delete g_pCameraReceiver;
        g_pCameraReceiver = NULL;
    }

    if (g_pUltrasoundManager1 != NULL)
    {
        g_pUltrasoundManager1->Stop(5);
        delete g_pUltrasoundManager1;
        g_pUltrasoundManager1 = NULL;
    }

    if (g_pUltrasoundManager2 != NULL)
    {
        g_pUltrasoundManager2->Stop(5);
        delete g_pUltrasoundManager2;
        g_pUltrasoundManager2 = NULL;
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

ResultEnum masterMain(const int cameraNo)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    unsigned char captureIndex = -1;
    unsigned char receiveIndex = -1;
    int key = 0;
    long cameraParameters[10] = { 100, 115, 100, 10, 3, 8, 168, 100, 10, 3 };
    Stopwatch watch;

    cv::Scalar                cvColor = cv::Scalar(0, 0, 256);
    cv::Point                cvPointStart = { 0x00 };

    if (masterInitialize(cameraNo) != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    try
    {
        std::ifstream readFile("/home/pi/FrontCameraMconfig.txt");
        std::string readData;
        if (readFile.fail())
        {
            g_pLogger->LOG_ERROR("[masterMain] config file read error.");
        }

        int lineCount = 0;
        while (getline(readFile, readData))
        {
            if (readData[0] == '#')
            {
                continue;
            }

            if (lineCount < 10)
            {
                cameraParameters[lineCount] = std::stol(readData);
            }

            lineCount++;
        }
    }
    catch(Exception e)
    {
        char logBuffer[50] = { 0 };
        snprintf(logBuffer, sizeof(logBuffer), "[masterMain] exception. [%s]", e.msg);
        g_pLogger->LOG_ERROR(logBuffer);
    }

    // g_pLogger->LOG_INFO("[masterMain] Main loop start.\n");
    watch.Start();
    while (1)
    {
        // 受信データの更新がある場合のみ、Master の持つ Camera 画像を取り込む
        captureIndex = pShareMemory->Capture.Index;
        Mat masterCapture = pShareMemory->Capture.Data[captureIndex];

        if (masterCapture.data != NULL)
        {
            bool isDetected = false;

            // 障害物検知 (現状は青色検知)
            cvColor = cv::Scalar(256, 0, 0);
            bool blueLineDetected = isColorLineDetected(masterCapture, cvColor, &cameraParameters[0]);
            if (blueLineDetected == true)
            {
                cvPointStart = Point(30, 80);
                putText(masterCapture, "blue detected.", cvPointStart, FONT_HERSHEY_SIMPLEX, 1.2, cvColor, 2, cv::FONT_HERSHEY_DUPLEX);

                // 障害物検知したので、1:回避指令
                pShareMemory->StateData = 1;
                isDetected = true;
            }
               
            // 障害物検知 (現状は赤色検知)
            cvColor = cv::Scalar(0, 0, 256);
            bool redLineDetected = isColorLineDetected(masterCapture, cvColor, &cameraParameters[5]);
            if (redLineDetected == true)
            {
                cvPointStart = Point(30, 30);
                putText(masterCapture, "red detected.", cvPointStart, FONT_HERSHEY_SIMPLEX, 1.2, cvColor, 2, cv::FONT_HERSHEY_DUPLEX);

                // 赤テープ検知したので、2:Uターン指令
                pShareMemory->StateData = 2;
                isDetected = true;
            }

            float distance = pShareMemory->UltrasoundData[1];
               
            cvPointStart = Point(30, 130);
            char logBuffer[50] = { 0 };
            snprintf(logBuffer, sizeof(logBuffer), "distance=%f", distance);
            putText(masterCapture, logBuffer, cvPointStart, FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(256, 256, 256), 2, cv::FONT_HERSHEY_DUPLEX);

            cv::imshow("camera", masterCapture);

            // 何も検知していなければ、0:指令無し
            if (isDetected == false)
            {
                pShareMemory->StateData = 0;
            }
        }

        key = cv::waitKey(1);
        if (key == 'q')
        {
            break;
        }

        if (300.0f <= watch.GetSplit())
        {
            int shutdown1 = digitalRead(IO_SHUTDOWN_1);
            int shutdown2 = digitalRead(IO_SHUTDOWN_2);
            if ((shutdown1 == LOW) && (shutdown2 == LOW))
            {
                break;
            }
        }
    }

    retVal = ResultEnum::NormalEnd;

FINISH :

    masterFinalize();

    return retVal;
}

bool isColorLineDetected(cv::Mat masterCapture, cv::Scalar cvColor, long* parameters)
{
    bool bRet = false;
    int width = masterCapture.cols;
    int height = masterCapture.rows;

    uchar hueMin = (uchar)parameters[0];
    uchar hueMax = (uchar)parameters[1];
    uchar saturation = (uchar)parameters[2];
    long widthThreshold = parameters[3];
    long heightThreshold = parameters[4];

    cv::Point         cvPointStart = { 0x00 };
    cv::Point         cvPointEnd = { 0x00 };
    cv::Mat            hsb;

    cvPointStart = cv::Point(640, 0);
    cvPointEnd = cv::Point(640, 480);

    cv::cvtColor(masterCapture, hsb, cv::COLOR_BGR2HSV);
    uchar hue, sat, val;

    Mat convertMat = Mat(Size(width, height), CV_8UC1);

    long detectCounter = 0;
    long widthCounter = 0;
    bool detected = false;

    int detectPointX = 0;
    int detectPointY = 0;

    /* 縦方向を走査 */
    for (int y = 0; y < height; y += 8)
    {
        bool lineDetected = false;

        /* 横方向を走査 */
        for (int x = 0; x < width; x += 8)
        {
            /* hue=色相を取得 */
            hue = hsb.at<Vec3b>(y, x)[0];

            /* sat=彩度を取得 */
            sat = hsb.at<Vec3b>(y, x)[1];

            /* val=明度を取得 */
            val = hsb.at<Vec3b>(y, x)[2];

            /* 160度以上が指定された場合、判定条件を変更する */
            if (hueMax > 160)
            {
                /* 指定の色相、彩度に合致するか判定 */
                if ((hue < hueMin || hue > hueMax) && sat > saturation)
                {
                    cvPointStart = cv::Point(x, y);
                    // cv::putText(masterCapture, ".", cvPointStart, FONT_HERSHEY_SIMPLEX, 1.2, cvColor, 2, cv::LINE_AA);
                    convertMat.at<uchar>(y, x) = 255;

                    /* 検知した横幅を測定する */
                    widthCounter++;
                }
                else
                {
                    convertMat.at<uchar>(y, x) = 0;

                    /* 検知無しの場合はカウンター初期化 */
                    widthCounter = 0;
                }
            }
            else
            {
                /* 指定の色相、彩度に合致するか判定 */
                if ((hue > hueMin && hue < hueMax) && sat > saturation)
                {
                    cvPointStart = Point(x, y);
                    // cv::putText(masterCapture, ".", cvPointStart, FONT_HERSHEY_SIMPLEX, 1.2, cvColor, 2, cv::LINE_AA);
                    convertMat.at<uchar>(y, x) = 255;
                   
                    /* 検知した横幅を測定する */
                    widthCounter++;
                }
                else
                {
                    convertMat.at<uchar>(y, x) = 0;
                    widthCounter = 0;
                }
            }

            /* 横幅の長さが一定値以上か確認 */
            if (widthCounter >= widthThreshold)
            {
                detectPointX = x;

                /* 横線を検知 */
                lineDetected = true;
            }
        }

        /* 横線を検知した回数をカウント */
        if (lineDetected == true)
        {
            detectCounter++;
        }
        else
        {
            detectCounter = 0;
        }

        /* 横線の検知回数が連続n回以上であれば、検知と判断 */
        if (detectCounter >= heightThreshold)
        {
            detectPointY = y;
            detected = true;
        }
    }

    if (detected == true)
    {
        rectangle(masterCapture, 
            cv::Point(detectPointX - (widthThreshold * 8), detectPointY - (heightThreshold * 8)),
            cv::Point(detectPointX, detectPointY),
            cvColor, 3);

        bRet = true;
    }

    return(bRet);
}

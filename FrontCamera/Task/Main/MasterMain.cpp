#include <stdlib.h>
#include "Include/Common.h"
#include "Logger/Logger.h"
#include "Socket/TcpClient/TcpClient.h"
#include "Socket/UdpReceiver/UdpReceiver.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Task/CameraCapture/CameraCapture.h"
#include "Task/CameraReceiver/CameraReceiver.h"
#include "Task/StateSender/StateSender.h"
#include "MasterMain.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

static Logger* g_pLogger = NULL;
static CameraCapture* g_pCameraCapture = NULL;
static CameraReceiver* g_pCameraReceiver = NULL;
static StateSender* g_pStateSender = NULL;

ResultEnum masterInitialize(const int cameraNo);
void masterFinalize();
bool isColorLineDetected(cv::Mat masterCapture, cv::Scalar cvColor, uchar hueMin, uchar hueMax, uchar saturation, long widthThreshold, long heightThreshold);

ResultEnum masterInitialize(const int cameraNo)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
	TcpClient* client = NULL;

    g_pLogger = new Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::LogTypeEnum::BOTH_OUT);
    if (g_pLogger == NULL)
    {
        goto FINISH;
    }

    ///* Slave からのカメラ画像を受信するスレッドの生成*/
    //g_pCameraReceiver = new CameraReceiver();
    //if (g_pCameraReceiver == NULL)
    //{
    //    g_pLogger->LOG_ERROR("[masterInitialize] g_pCameraReceiver allocation failed.\n");
    //    goto FINISH;
    //}

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

    //g_pCameraReceiver->Run();
    g_pCameraCapture->Run();
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

	cv::Scalar				cvColor = cv::Scalar(0, 0, 256);
	cv::Point				cvPointStart = { 0x00 };

    if (masterInitialize(cameraNo) != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    g_pLogger->LOG_INFO("[masterMain] Main loop start.\n");
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
            bool blueLineDetected = isColorLineDetected(masterCapture, cvColor, 100, 115, 100, 10, 3);
            if (blueLineDetected == true)
            {
                cvPointStart = Point(30, 80);
                putText(masterCapture, "blue detected.", cvPointStart, FONT_HERSHEY_SIMPLEX, 1.2, cvColor, 2, cv::FONT_HERSHEY_DUPLEX);

                // 障害物検知したので、1:回避指令
                g_pStateSender->SetState(1);
                isDetected = true;
            }
               
            // 障害物検知 (現状は赤色検知)
            cvColor = cv::Scalar(0, 0, 256);
			bool redLineDetected = isColorLineDetected(masterCapture, cvColor, 8, 168, 100, 10, 3);
			if (redLineDetected == true)
			{
				cvPointStart = Point(30, 30);
				putText(masterCapture, "red detected.", cvPointStart, FONT_HERSHEY_SIMPLEX, 1.2, cvColor, 2, cv::FONT_HERSHEY_DUPLEX);

                // 赤テープ検知したので、2:Uターン指令
                g_pStateSender->SetState(2);
                isDetected = true;
			}
               
            cv::imshow("camera", masterCapture);

            // 何も検知していなければ、0:指令無し
            if (isDetected == false)
            {
                g_pStateSender->SetState(0);
            }
		}

        key = cv::waitKey(1);
        if (key == 'q')
        {
            break;
        }
    }

    retVal = ResultEnum::NormalEnd;

FINISH :

    masterFinalize();
    return retVal;
}

bool isColorLineDetected(cv::Mat masterCapture, cv::Scalar cvColor, uchar hueMin, uchar hueMax, uchar saturation, long widthThreshold, long heightThreshold)
{
	bool bRet = false;
	int width = masterCapture.cols;
	int height = masterCapture.rows;

	cv::Point         cvPointStart = { 0x00 };
	cv::Point         cvPointEnd = { 0x00 };
	cv::Mat			hsb;

	cvPointStart = cv::Point(640, 0);
	cvPointEnd = cv::Point(640, 480);

	cv::cvtColor(masterCapture, hsb, cv::COLOR_BGR2HSV);
	uchar hue, sat, val;

	Mat convertMat = Mat(Size(width, height), CV_8UC1);

	long detectCounter = 0;
	long totalCounter = 0;
	long widthCounter = 0;
	bool redDetected = false;

	for (int y = 0; y < height; y += 8)
	{
		bool redLineDetected = false;
		for (int x = 0; x < width; x += 8)
		{
			hue = hsb.at<Vec3b>(y, x)[0];
			sat = hsb.at<Vec3b>(y, x)[1];
			val = hsb.at<Vec3b>(y, x)[2];
			if (hueMax > 160)
			{
				if ((hue < hueMin || hue > hueMax) && sat > saturation)
				{
					cvPointStart = cv::Point(x, y);
					cv::putText(masterCapture, ".", cvPointStart, FONT_HERSHEY_SIMPLEX, 1.2, cvColor, 2, cv::LINE_AA);
					convertMat.at<uchar>(y, x) = 255;
					widthCounter++;
					totalCounter++;
				}
				else
				{
					convertMat.at<uchar>(y, x) = 0;
					widthCounter = 0;
				}
			}
			else
			{
				if ((hue > hueMin && hue < hueMax) && sat > saturation)
				{
					cvPointStart = Point(x, y);
					cv::putText(masterCapture, ".", cvPointStart, FONT_HERSHEY_SIMPLEX, 1.2, cvColor, 2, cv::LINE_AA);
					convertMat.at<uchar>(y, x) = 255;
					widthCounter++;
					totalCounter++;
				}
				else
				{
					convertMat.at<uchar>(y, x) = 0;
					widthCounter = 0;
				}
			}

			if (widthCounter >= widthThreshold)
			{
				redLineDetected = true;
			}
		}

		if (redLineDetected == true)
		{
			detectCounter++;
		}
		else
		{
			detectCounter = 0;
		}

		if (detectCounter >= heightThreshold)
		{
			redDetected = true;
		}
	}

	if (redDetected == true)
	{
		bRet = true;
	}

	return(bRet);
}

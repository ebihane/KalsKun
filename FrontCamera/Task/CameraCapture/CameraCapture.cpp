#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Parts/ShareMemory/ShareMemory.h"
#include "CameraCapture.h"

CameraCapture::CameraCapture(const int index)
 : ThreadBase((char*)"CameraCapture")
 , m_CameraIndex(index)
 , m_Start(false)
 , m_Capture(NULL)
{
    m_CameraParameters[0] = 100;
    m_CameraParameters[1] = 115;
    m_CameraParameters[2] = 100;
    m_CameraParameters[3] = 10;
    m_CameraParameters[4] = 3;
    m_CameraParameters[5] = 8;
    m_CameraParameters[6] = 168;
    m_CameraParameters[7] = 100;
    m_CameraParameters[8] = 10;
    m_CameraParameters[9] = 3;
}

CameraCapture::~CameraCapture()
{
    finalize();
}

bool CameraCapture::IsCaptureStart()
{
    return m_Start;
}

ResultEnum CameraCapture::initialize()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    m_Start = false;

    try
    {
        std::ifstream readFile("/home/pi/FrontCameraMconfig.txt");
        std::string readData;
        if (readFile.fail())
        {
            m_Logger->LOG_ERROR("[CameraCapture::initialize] config file read error.");
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
                m_CameraParameters[lineCount] = std::stol(readData);
            }

            lineCount++;
        }
    }
    catch (Exception& e)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[CameraCapture::initialize] Exception! [%s]", e.what());
        m_Logger->LOG_ERROR(m_LogStr);
    }

    retVal = ResultEnum::NormalEnd;

    return retVal;
}

ResultEnum CameraCapture::doProcedure()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    long       nextIndex = 0;
    cv::Scalar cvColor = cv::Scalar(0, 0, 256);
    cv::Point  cvPointStart = { 0x00 };

RETRY:

    m_Start = false;

    if (m_Capture != NULL)
    {
        delete m_Capture;
        m_Capture = NULL;
    }

    m_Capture = new cv::VideoCapture(m_CameraIndex);
    if (m_Capture == NULL)
    {
        m_Logger->LOG_ERROR("[CameraCapture] m_Capture allocation failed.\n");
        goto FINISH;
    }

    if (m_Capture->isOpened() == false)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[CameraCapture] Camera No.[%ld] Open failed.\n", m_CameraIndex);
        m_Logger->LOG_ERROR(m_LogStr);

        if (isStopRequest() == true)
        {
            m_Logger->LOG_INFO("[CameraCapture] Thread stop request.\n");
            goto FINISH;
        }

        m_CameraIndex++;
        if (100 <= m_CameraIndex)
        {
            m_CameraIndex = 0;
        }

        delay(100);
        goto RETRY;
    }

    /* サイズの指定 */
    m_Capture->set(CAP_PROP_FRAME_WIDTH, 640);
    m_Capture->set(CAP_PROP_FRAME_HEIGHT, 480);

    /* 最初の画像取り込み */
    pShareMemory->Capture.Index = 0;
    m_Capture->read(pShareMemory->Capture.Data[0]);
    m_Start = true;
    nextIndex = 1;

    snprintf(&m_LogStr[0], sizeof(m_LogStr), "[CameraCapture] Main loop start. CameraNo[%d]\n", m_CameraIndex);
    m_Logger->LOG_INFO(m_LogStr);
    while (1)
    {
        /* 停止要求 */
        if (isStopRequest() == true)
        {
            m_Logger->LOG_INFO("[CameraCapture] Stop request.\n");
            break;
        }

        /* カメラ画像取り込み */
        if (m_Capture->read(pShareMemory->Capture.Data[nextIndex]) == false)
        {
            m_Logger->LOG_ERROR("[CameraCapture] Camera read failed.\n");
            pShareMemory->SystemError = true;
            goto RETRY;
        }

        // 障害物検知 (現状は青)
        cvColor = cv::Scalar(256, 0, 0);
        bool blueLineDetected = isColorLineDetected(pShareMemory->Capture.Data[nextIndex], cvColor, &m_CameraParameters[0]);
        if (blueLineDetected == false)
        {
            pShareMemory->BlueObject = DetectTypeEnum::NOT_DETECT;
        }
        else
        {
            cvPointStart = Point(30, 80);
            putText(pShareMemory->Capture.Data[nextIndex], "blue detected.", cvPointStart, FONT_HERSHEY_SIMPLEX, 1.2, cvColor, 2, cv::FONT_HERSHEY_DUPLEX);
            pShareMemory->BlueObject = DetectTypeEnum::DETECTED;
        }

        // 赤テープ
        cvColor = cv::Scalar(0, 0, 256);
        bool redLineDetected = isColorLineDetected(pShareMemory->Capture.Data[nextIndex], cvColor, &m_CameraParameters[5]);
        if (redLineDetected == false)
        {
            pShareMemory->RedTape = DetectTypeEnum::NOT_DETECT;
        }
        else
        {
            cvPointStart = Point(30, 30);
            putText(pShareMemory->Capture.Data[nextIndex], "red detected.", cvPointStart, FONT_HERSHEY_SIMPLEX, 1.2, cvColor, 2, cv::FONT_HERSHEY_DUPLEX);
            pShareMemory->RedTape = DetectTypeEnum::DETECTED;
        }

        for (int index = 0; index < 2; index++)
        {
            float distance = pShareMemory->UltrasoundData[index];
            char distanseStr[50] = { 0 };

            cvPointStart = Point(30, 130 + (50 * index));

            snprintf(&distanseStr[0], sizeof(distanseStr), "distance%d=%f", index + 1, distance);
            putText(pShareMemory->Capture.Data[nextIndex], &distanseStr[0], cvPointStart, FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(256, 256, 256), 2, cv::FONT_HERSHEY_DUPLEX);
        }

        pShareMemory->Capture.Index = nextIndex;
        nextIndex++;
        if (CAMERA_DATA_MAX <= nextIndex)
        {
            nextIndex = 0;
        }

        delay(10);
    }


FINISH :
    m_Logger->LOG_INFO("[CameraCapture]::Exit.\n");
    return retVal;
}

ResultEnum CameraCapture::finalize()
{
    m_Start = false;

    if (m_Capture != NULL)
    {
        delete m_Capture;
        m_Capture = NULL;
    }

    return ResultEnum::NormalEnd;
}

bool CameraCapture::isColorLineDetected(cv::Mat& masterCapture, cv::Scalar cvColor, long* parameters)
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
    uchar hue, sat;

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

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
    /* nop. */
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
    m_HumanFaceCascade = cv::CascadeClassifier("/home/pi/haarcascade_frontalface_alt.xml");
    m_AnimalCascade = cv::CascadeClassifier("/home/pi/haarcascade_frontalcatface.xml");
    m_AnimalCascade2 = cv::CascadeClassifier("/home/pi/haarcascade_frontalcatface_extended.xml");

    retVal = ResultEnum::NormalEnd;

    return retVal;
}

ResultEnum CameraCapture::doProcedure()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    long       nextIndex = 0;
    cv::Scalar cvColor = cv::Scalar(0, 0, 256);
    cv::Point   cvPointStart = { 0x00 };

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

        /* 顔検知 */
        bool faceDetected = isDetect(pShareMemory->Capture.Data[nextIndex], m_HumanFaceCascade);
        if (faceDetected == true)
        {
            cvPointStart = Point(30, 130);
            cvColor = cv::Scalar(256, 256, 256);
            putText(pShareMemory->Capture.Data[nextIndex], "face detected.", cvPointStart, FONT_HERSHEY_SIMPLEX, 1.2, cvColor, 2, cv::FONT_HERSHEY_DUPLEX);

            // 不審者検知
            pShareMemory->Human = DetectTypeEnum::DETECTED;
        }
        else
        {
            // 不審者非検知
            pShareMemory->Human = DetectTypeEnum::NOT_DETECT;
        }

        /* 動物検知 */
        bool animalDetected = isDetect(pShareMemory->Capture.Data[nextIndex], m_AnimalCascade);
        bool animal2Detected = isDetect(pShareMemory->Capture.Data[nextIndex], m_AnimalCascade2);
        if ((animalDetected == true) || (animal2Detected == true))
        {
            cvPointStart = Point(30, 180);
            cvColor = cv::Scalar(256, 256, 256);
            putText(pShareMemory->Capture.Data[nextIndex], "Animal detected.", cvPointStart, FONT_HERSHEY_SIMPLEX, 1.2, cvColor, 2, cv::FONT_HERSHEY_DUPLEX);

            // 動物検知
            pShareMemory->Animal = DetectTypeEnum::DETECTED;
        }
        else
        {
            // 動物非検知
            pShareMemory->Animal = DetectTypeEnum::NOT_DETECT;
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

    return ResultEnum::NormalEnd;
}

bool CameraCapture::isDetect(cv::Mat& masterCapture, cv::CascadeClassifier& cascade)
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


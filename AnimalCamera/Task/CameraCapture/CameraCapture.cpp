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
    if (m_Capture != NULL)
    {
        delete m_Capture;
        m_Capture = NULL;
    }
}

bool CameraCapture::IsCaptureStart()
{
    return m_Start;
}

ResultEnum CameraCapture::initialize()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    m_Start = false;
    //	cascade = cv::CascadeClassifier("/home/pi/haarcascade_lowerbody.xml");
    //	cascade = cv::CascadeClassifier("/home/pi/haarcascade_frontalcatface.xml");
    m_Cascade = cv::CascadeClassifier("/home/pi/haarcascade_frontalface_alt.xml");

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
    if (m_Capture == NULL)
    {
        m_Capture = new cv::VideoCapture(m_CameraIndex);
        if (m_Capture == NULL)
        {
            m_Logger->LOG_ERROR("[CameraCapture] m_Capture allocation failed.\n");
            goto FINISH;
        }
    }

    m_Capture->set(CAP_PROP_FRAME_WIDTH, 640);
    m_Capture->set(CAP_PROP_FRAME_HEIGHT, 480);

    if (m_Capture->isOpened() == false)
    {
        if (m_Capture != NULL)
        {
            delete m_Capture;
            m_Capture = NULL;
        }

        delay(100);

        m_Logger->LOG_ERROR("[CameraCapture] isOpened is false.\n");
        goto RETRY;
    }

    pShareMemory->Capture.Index = 0;
    m_Capture->read(pShareMemory->Capture.Data[0]);
    m_Start = true;

    nextIndex = 1;
    while (1)
    {
        if (isStopRequest() == true)
        {
            break;
        }

        if (m_Capture->read(pShareMemory->Capture.Data[nextIndex]) == false)
        {
            m_Logger->LOG_ERROR("[CameraCapture] Camera read failed.\n");
            pShareMemory->SystemError = true;
            goto FINISH;
        }

        bool faceDetected = isFaceExist(pShareMemory->Capture.Data[nextIndex], m_Cascade);
        if (faceDetected == true)
        {
            cvPointStart = Point(30, 130);
            cvColor = cv::Scalar(256, 256, 256);
            putText(pShareMemory->Capture.Data[nextIndex], "face detected.", cvPointStart, FONT_HERSHEY_SIMPLEX, 1.2, cvColor, 2, cv::FONT_HERSHEY_DUPLEX);

            // 不審者検知
            pShareMemory->Detection = 1;
        }
        else
        {
            // 不審者非検知
            pShareMemory->Detection = 0;
        }


        pShareMemory->Capture.Index = nextIndex;

        nextIndex++;
        if (CAMERA_DATA_MAX <= nextIndex)
        {
            nextIndex = 0;
        }
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

bool CameraCapture::isFaceExist(cv::Mat masterCapture, cv::CascadeClassifier cascade)
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


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Parts/ShareMemory/ShareMemory.h"
#include "CameraCapture.h"

CameraCapture::CameraCapture(const int index)
 : ThreadBase()
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

    m_Logger = new Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::LogTypeEnum::BOTH_OUT);
    if (m_Logger == NULL)
    {
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

ResultEnum CameraCapture::doProcedure()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    long       nextIndex = 0;


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
        m_Logger->LOG_ERROR("[CameraCapture] isOpened is false.\n");
        goto FINISH;
    }

    m_Capture->read(pShareMemory->Capture.Data[0]);
    m_Start = true;

    pShareMemory->Capture.Index = 0;
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

        // @todo : ‚±‚±‚É‰æ‘œ•â³‚ð“ü‚ê‚é

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

    if (m_Logger != NULL)
    {
        delete m_Logger;
        m_Logger = NULL;
    }

    return ResultEnum::NormalEnd;
}
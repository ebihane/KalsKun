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

    retVal = ResultEnum::NormalEnd;

    return retVal;
}

ResultEnum CameraCapture::doProcedure()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    long       nextIndex = 0;

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

    m_Capture->read(pShareMemory->Capture.Data[0]);
    m_Start = true;

    pShareMemory->Capture.Index = -1;
    nextIndex = 0;
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

        // @todo : �����ɉ摜�␳������

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
#include "Parts/ShareMemory/ShareMemory.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
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

    retVal = ResultEnum::NormalEnd;

    return retVal;
}

ResultEnum CameraCapture::doProcedure()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    long nextIndex = 0;
    long detectState = 0;
    long beforeDetectState = 0;

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
    pShareMemory->PatrolState = E_PATROL_NONE;

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

        // ‰æ‘œ•â³•K—vH


        /* ˜^‰æŽÀŽ{”»’è */
        detectState = pShareMemory->PatrolState;
        if (detectState == E_PATROL_DETECT)
        {
            if (beforeDetectState == E_PATROL_DETECT)
            {
                if (startVideoWrite() != true)
                {
                    m_Logger->LOG_ERROR("[CameraCapture] startVideoWrite failed.\n");
                }
            }

            (*m_VideoWriter) << pShareMemory->Capture.Data[nextIndex];
        }
        else
        {
            if (beforeDetectState == E_PATROL_DETECT)
            {
                stopVideoWrite();
            }
        }

        beforeDetectState = detectState;

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

    stopVideoWrite();

    if (m_Capture != NULL)
    {
        m_Capture->release();
        delete m_Capture;
    }

    return ResultEnum::NormalEnd;
}

bool CameraCapture::startVideoWrite()
{
    bool retVal = false;
    time_t nowTime = -1;
    struct tm* tmPtr = NULL;
    char folderPath[80] = { 0 };
    int fourcc = VideoWriter::fourcc('W', 'M', 'V', '1');

    if (m_VideoWriter != NULL)
    {
        retVal = true;
        goto FINISH;
    }

    nowTime = time(NULL);
    tmPtr = localtime(&nowTime);
    snprintf(&folderPath[0], sizeof(folderPath), "/home/pi/20%02d%02d%02d_%02d%02d%02d.wmv",
        tmPtr->tm_year - 100, tmPtr->tm_mon + 1, tmPtr->tm_mday, tmPtr->tm_hour, tmPtr->tm_min, tmPtr->tm_sec);

    m_VideoWriter = new VideoWriter(folderPath, fourcc, 30, cv::Size(640, 480), true);
    if (m_VideoWriter == NULL)
    {
        goto FINISH;
    }

    retVal = true;

FINISH :
    return retVal;
}

void CameraCapture::stopVideoWrite()
{
    if (m_VideoWriter != NULL)
    {
        m_VideoWriter->release();
        delete m_VideoWriter;
    }
}

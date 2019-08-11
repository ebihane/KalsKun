#include "Parts/ShareMemory/ShareMemory.h"
#include "KinectCapture.h"

// Memo :
//  Kinect を接続しているUSBのパーミッションを変更する必要あり
//  sudo chmod 777 /dev/bus/usb/001/*
//  恒久的に対応するための方法もあり (下記ブログのPatrick氏のコメント参照)
//  http://blog.bitcollectors.com/adam/2016/01/kinect-support-for-raspberry-pi-using-libfreenect/

using namespace cv;

Mat KinectCapture::m_DepthImage;

void depthCallback(freenect_device* dev, void* depth, uint32_t timestamp)
{
    Mat d8;

    // 深度画像データを入れる
    Mat temp = Mat(480, 640, CV_16UC1, depth);

    // 8bitグレースケール画像に変換, 4000は測定可能な最大距離
    temp.convertTo(d8, CV_8UC1, -255.0f / 4000, 255.0f);

    // データをコピー
    int index = pShareMemory->Capture.Index + 1;
    if (CAMERA_DATA_MAX <= index)
    {
        index = 0;
    }

    memcpy(pShareMemory->Capture.Data[index].data, d8.data, 640 * 480);
    pShareMemory->Capture.Index = index;
}

KinectCapture::KinectCapture()
 : m_Handle(-1)
 , m_DepthMutex(PTHREAD_MUTEX_INITIALIZER)
 , m_RgbMutex(PTHREAD_MUTEX_INITIALIZER)
 , m_Context(NULL)
 , m_Device(NULL)
{
    /* nop. */
}

KinectCapture::~KinectCapture()
{
    finalize();
}

ResultEnum KinectCapture::Start()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    long index = 0;
    pthread_t tid = -1;
    pthread_attr_t  attr;

    m_Logger = new Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::LogTypeEnum::BOTH_OUT);
    if (m_Logger == NULL)
    {
        goto FINISH;
    }

    if (initialize() != true)
    {
        m_Logger->LOG_ERROR("[KinectCapture] initialize failed.\n");
        goto FINISH;
    }

    pShareMemory->Capture.Index = -1;
    for (index = 0; index < CAMERA_DATA_MAX; index++)
    {
        pShareMemory->Capture.Data[index] = Mat(FREENECTOPENCV_DEPTH_HEIGHT, FREENECTOPENCV_DEPTH_WIDTH, CV_8UC1);
    }

    if (pthread_attr_init(&attr) != 0)
    {
        goto FINISH;
    }

    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
    {
        goto FINISH;
    }

    if (pthread_create(&tid, &attr, this->threadMain, this) != 0)
    {
        goto FINISH;
    }

    m_Handle = (int)tid;

    retVal = ResultEnum::NormalEnd;


FINISH :
    return retVal;
}

ResultEnum KinectCapture::Stop()
{
    m_StopRequest = true;

    ResultEnum retVal = ResultEnum::AbnormalEnd;
    unsigned long cnt = 0;

    m_StopRequest = true;
    while (1)
    {
        if (10000 < cnt)
        {
            goto FINISH;
        }

        if (m_Running != true)
        {
            break;
        }

        cnt++;
        delay(100);
    }

    retVal = ResultEnum::NormalEnd;


FINISH:
    return retVal;
}

void* KinectCapture::threadMain(void* param)
{
    KinectCapture* p = (KinectCapture*)param;

    p->mainProcedure();

    return NULL;
}

bool KinectCapture::initialize()
{
    bool retVal = false;

    if (freenect_init(&m_Context, NULL) < 0)
    {
        m_Logger->LOG_ERROR("[KinectCapture] freenect_init failed.\n");
        goto FINISH;
    }

    if (freenect_open_device(m_Context, &m_Device, 0) < 0)
    {
        m_Logger->LOG_ERROR("[KinectCapture] freenect_open_device failed.\n");
        goto FINISH;
    }

    freenect_set_depth_callback(m_Device, depthCallback);

    retVal = true;

FINISH :
    return retVal;
}

void KinectCapture::mainProcedure()
{
    m_Running = true;

    freenect_start_depth(m_Device);
    while (freenect_process_events(m_Context) >= 0)
    {
        if (m_StopRequest == true)
        {
            break;
        }
    }

    finalize();

    m_Running = false;
}

void KinectCapture::finalize()
{
    if (m_Device != NULL)
    {
        freenect_stop_depth(m_Device);
    }
}


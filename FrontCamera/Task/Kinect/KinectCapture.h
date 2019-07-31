#pragma once
#include <opencv2/opencv.hpp>
#include <libfreenect.h>
#include <pthread.h>
#include "Include/Common.h"
#include "Logger/Logger.h"

class KinectCapture
{
public:

    KinectCapture();
    virtual ~KinectCapture();

    ResultEnum Start();
    ResultEnum Stop();

protected:


private:

    static const long FREENECTOPENCV_RGB_DEPTH = 3;
    static const long FREENECTOPENCV_DEPTH_DEPTH = 1;
    static const long FREENECTOPENCV_RGB_WIDTH = 640;
    static const long FREENECTOPENCV_RGB_HEIGHT = 480;
    static const long FREENECTOPENCV_DEPTH_WIDTH = 640;
    static const long FREENECTOPENCV_DEPTH_HEIGHT = 480;

    static cv::Mat m_DepthImage;

    static void* threadMain(void* param);

    int m_Handle;
    bool m_StopRequest;
    bool m_Running;

    pthread_mutex_t m_DepthMutex;
    pthread_mutex_t m_RgbMutex;

    freenect_context* m_Context;
    freenect_device* m_Device;

    Logger* m_Logger;

    bool initialize();
    void mainProcedure();
    void finalize();

};
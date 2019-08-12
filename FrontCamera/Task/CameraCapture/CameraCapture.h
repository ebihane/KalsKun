#pragma once

#include <opencv2/opencv.hpp>
#include "ThreadBase/ThreadBase.h"
#include "Logger/Logger.h"

class CameraCapture : public ThreadBase
{
public :

    CameraCapture(const int index);
    virtual ~CameraCapture();

    bool IsCaptureStart();

protected :


private :

    int m_CameraIndex;
    bool m_Start;
    cv::VideoCapture* m_Capture;

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

};

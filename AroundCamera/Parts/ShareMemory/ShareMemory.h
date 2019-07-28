#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;

#define CAMERA_DATA_MAX (10)

typedef struct
{
    unsigned char Index;
    Mat Data[CAMERA_DATA_MAX];
} CameraDataStr;

typedef struct
{
    bool SystemError;
    bool IsVideoRecord;
    CameraDataStr Capture;
} ShareMemoryStr;

#ifdef MEMORY_MAIN
ShareMemoryStr* pShareMemory;
#else
extern ShareMemoryStr* pShareMemory;
#endif
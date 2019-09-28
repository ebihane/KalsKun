#pragma once

#include <opencv2/opencv.hpp>
using namespace cv;

#define CAMERA_DATA_MAX (10)

/* ���m��Ԓ萔 */
typedef enum
{
    NOT_DETECT = 0,
    DETECTED,
} DetectTypeEnum;

typedef struct
{
    unsigned char Index;
    Mat Data[CAMERA_DATA_MAX];
} CameraDataStr;

typedef struct
{
    bool SystemError;
    DetectTypeEnum Detect;
    CameraDataStr Capture;
} ShareMemoryStr;

#ifdef MEMORY_MAIN
ShareMemoryStr* pShareMemory;
#else
extern ShareMemoryStr* pShareMemory;
#endif

#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;

#define CAMERA_DATA_MAX (10)

typedef enum
{
    E_PATROL_NONE = 0,
    E_PATROL_NORMAL,
    E_PATROL_FIRST_DETECT,
    E_PATROL_DETECTING,
    E_PATROL_DETECT,
} PatrolStateEnum;

typedef struct
{
    unsigned char Index;
    Mat Data[CAMERA_DATA_MAX];
} CameraDataStr;

typedef struct
{
    bool SystemError;
    PatrolStateEnum PatrolState;
    CameraDataStr Capture;
} ShareMemoryStr;

#ifdef MEMORY_MAIN
ShareMemoryStr* pShareMemory;
#else
extern ShareMemoryStr* pShareMemory;
#endif

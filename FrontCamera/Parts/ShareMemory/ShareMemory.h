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

/* ����w���萔 */
typedef enum
{
    NOT_REQUEST = 0,    /* 0: �v������ */
    AVOIDANCE,          /* 1: ����w�� */
    TURN,               /* 2: �^�[���w�� */
} MoveTypeEnum;

typedef struct
{
    unsigned char Index;
    Mat Data[CAMERA_DATA_MAX];
} CameraDataStr;

typedef struct
{
    bool SystemError;
    MoveTypeEnum MoveType;
    DetectTypeEnum RedTape;
    DetectTypeEnum BlueObject;
    float UltrasoundData[2];
    CameraDataStr Capture;
    CameraDataStr Communicate;
} ShareMemoryStr;

#ifdef MEMORY_MAIN
    ShareMemoryStr* pShareMemory;
#else
    extern ShareMemoryStr* pShareMemory;
#endif
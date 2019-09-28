#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;

#define CAMERA_DATA_MAX (10)

/* 検知状態定数 */
typedef enum
{
    NOT_DETECT = 0,
    DETECTED,
} DetectTypeEnum;

/* 動作指示定数 */
typedef enum
{
    NOT_REQUEST = 0,    /* 0: 要求無し */
    AVOIDANCE,          /* 1: 回避指示 */
    TURN,               /* 2: ターン指示 */
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
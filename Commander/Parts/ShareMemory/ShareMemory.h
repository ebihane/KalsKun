#pragma once

typedef struct
{
    long ReceiveCount;  /* 受信回数 */
    long SystemError;   /* システムエラー状態 */
    long PersonDetect;  /* 人検知状態 */
} AroundCameraStateStr;

typedef struct
{
    char LeftRight;     /* 左右 */
    char FrontBack;     /* 前後 */
    char CutterSpeed;   /* 草刈り刃スピード */
} MotorCommandStr;

typedef struct
{
    AroundCameraStateStr AroundCamera;
    MotorCommandStr MotorCommand;
} ShareMemoryStr;

#ifdef MEMORY_MAIN
    ShareMemoryStr* pShareMemory;
#else
    extern ShareMemoryStr* pShareMemory;
#endif

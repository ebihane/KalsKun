#pragma once

typedef struct
{
    long ReceiveCount;  /* 受信回数 */
    long SystemError;   /* システムエラー状態 */
    long PersonDetect;  /* 人検知状態 */
} AroundCameraState;

typedef struct
{
    AroundCameraState AroundCamera;
} ShareMemoryStr;

#ifdef MEMORY_MAIN
    ShareMemoryStr* pShareMemory;
#else
    extern ShareMemoryStr* pShareMemory;
#endif

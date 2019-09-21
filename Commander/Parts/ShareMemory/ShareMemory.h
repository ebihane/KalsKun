#pragma once

/* ビーコン個数 */
#define BEACON_COUNT    (3)

/* 方向定数 (値は時計回り) */
typedef enum
{
    E_DIR_FRONT = 0,    /* 0: 前進 */
    E_DIR_FRONT_R,      /* 1: 右斜め前 */
    E_DIR_RIGHT,        /* 2: 右 */
    E_DIR_BACK_R,       /* 3: 右斜め後ろ */
    E_DIR_BACK,         /* 4: 後ろ */
    E_DIR_BACK_L,       /* 5: 左斜め後ろ */
    E_DIR_LEFT,         /* 6: 左 */
    E_DIR_FRONT_L,      /* 7: 左斜め前 */
    E_DIR_TYPE_MAX,     /* 8: 方向種別数 */
} DirectionEnum;

/* 司令塔マイコン 電波強度関連情報 */
typedef struct
{
    float   RssiAverage;
    float   TxPowerAverage;
    float   Distance;
} BeaconDataStr;


/* 遠隔操作関連 */
typedef enum
{
    E_MODE_MANUAL = 0,  /* 0: 手動 */
    E_MODE_AUTO,        /* 1: 自動 */
} MoveTypeEnum;

/* リモコン */
typedef struct
{
    MoveTypeEnum Mode;  /* 制御モード */

} MoveControlStr;






/* 前方カメラ 状態 */
typedef struct
{
    long ReceiveCount;  /* 受信回数 */
    long SystemError;   /* システムエラー状態 */
    long Avoidance;     /* 回避指示 */
} FrontCameraStateStr;

/* 周辺カメラ 状態 */
typedef struct
{
    long ReceiveCount;  /* 受信回数 */
    long SystemError;   /* システムエラー状態 */
    long PersonDetect;  /* 人検知状態 */
} AroundCameraStateStr;

/* モータマイコン通信 指示内容 */
typedef struct
{
    char LeftRight;     /* 左右 */
    char FrontBack;     /* 前後 */
    char CutterSpeed;   /* 草刈り刃スピード */
} MotorCommandStr;




/*-----------------*/
/* 共有メモリ 本体 */
/*-----------------*/
typedef struct
{
    AroundCameraStateStr    AroundCamera;
    FrontCameraStateStr     FrontCamera;

    MoveControlStr          MoveControl;

    MotorCommandStr         MotorCommand;
    BeaconDataStr           BeaconData[BEACON_COUNT];
} ShareMemoryStr;

#ifdef MEMORY_MAIN
    ShareMemoryStr* pShareMemory;
#else
    extern ShareMemoryStr* pShareMemory;
#endif

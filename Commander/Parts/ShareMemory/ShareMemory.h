#pragma once

/* 方角定数 */
typedef enum
{
    E_COMPASS_NORTH = 0,    /* 0: 北 */
    E_COMPASS_NORTHEAST,    /* 1: 北東 */
    E_COMPASS_EAST,         /* 2: 東 */
    E_COMPASS_SOUTHEAST,    /* 3: 南東 */
    E_COMPASS_SOUTH,        /* 4: 南 */
    E_COMPASS_SOUTHWEST,    /* 5: 南西 */
    E_COMPASS_WEST,         /* 6: 西 */
    E_COMPASS_NORTHWEST,    /* 7: 北西 */
    E_COMPASS_MAX,          /* 8: 方角数 */
} CompassEnum;

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

/* 位置情報 */
typedef struct
{
    unsigned long   CurrentX;   /* 現在座標 X */
    unsigned long   CurrentY;   /* 現在座標 Y */
} PositionStr;

typedef struct
{
    AroundCameraStateStr    AroundCamera;
    MotorCommandStr         MotorCommand;
    PositionStr             Position;
} ShareMemoryStr;

#ifdef MEMORY_MAIN
    ShareMemoryStr* pShareMemory;
#else
    extern ShareMemoryStr* pShareMemory;
#endif

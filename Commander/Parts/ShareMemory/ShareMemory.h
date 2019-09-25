#pragma once


/*==================*/
/*  司令塔マイコン  */
/*==================*/
/* ブザー吹鳴モード */
typedef enum
{
    E_MELODY_SILENT = 0,    /* 0: 停止 */
    E_MELODY_SOUND_1,       /* 1: 曲 1 */
    E_MELODY_WARNING,       /* 2: 警告 */
    E_MELODY_ERROR,         /* 3: 異常発生 */
} MelodyModeEnum;

/* 指向性ライトモード */
typedef enum
{
    E_LIGHT_OFF = 0,        /* 0: OFF */
    E_LIGHT_ON,             /* 1: ON */
    E_LIGHT_BLINK,          /* 2: 点滅 */
} LightModeEnum;

/*----------------*/
/*  位置情報関連  */
/*----------------*/
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

/* 電波強度関連情報 */
typedef struct
{
    float   RssiAverage;
    float   TxPowerAverage;
    float   Distance;
} BeaconDataStr;

/* 司令塔マイコン 状態 */
typedef struct
{
    BeaconDataStr   Beacon[BEACON_COUNT];
    MelodyModeEnum  MelodyMode;
    LightModeEnum   LightMode;
    long            SystemError;
} CommanderStateStr;

/*==================*/
/*  モータマイコン  */
/*==================*/
/* モータマイコン向け 動作指示 */
typedef enum
{
    E_COMMAND_STOP = 0, /* 0: 停止 */
    E_COMMAND_FRONT,    /* 1: 前進 */
    E_COMMAND_U_TURN,   /* 2: Uターン */
    E_COMMAND_AVOID,    /* 3: 回避 */
    E_COMMAND_REMOTE,   /* 4: 遠隔動作 */
    E_COMMAND_MONITOR,  /* 5: モニタ */
    E_COMMAND_MAX,      /* 6: コマンド数 */
} MotorCommandEnum;

/* モータマイコン向け 草刈り刃動作指示 */
typedef enum
{
    E_CUTTER_STOP = 0,  /* 0: 草刈り刃停止 */
    E_CUTTER_DRIVE,     /* 1: 草刈り刃駆動 */
    E_CUTTER_TYPE_MAX,  /* 2: 草刈り刃 制御数 */
} CutterDriveEnum;

/* 遠隔操作モード */
typedef enum
{
    E_MODE_MANUAL = 0,  /* 0: 手動 */
    E_MODE_AUTO,        /* 1: 自動 */
} ControlModeEnum;

/* モータマイコン 状態 */
typedef struct
{
    MotorCommandEnum    Command;
    CutterDriveEnum     Cutter;
    long                PointX;
    long                PointY;
    long                ErrorStatus;
    ControlModeEnum     RemoteMode;
} MotorStatusStr;

/*==============*/
/*  前方カメラ  */
/*==============*/
/* 前方カメラ 状態 */
typedef struct
{
    long ReceiveCount;  /* 受信回数 */
    long SystemError;   /* システムエラー状態 */
    long Avoidance;     /* 回避指示 */
    float Distance[2];  /* 障害物との距離 */
} FrontCameraStateStr;

/*==============*/
/*  動物カメラ  */
/*==============*/
/* 動物カメラ 状態 */
typedef struct
{
    long ReceiveCount;  /* 受信回数 */
    long SystemError;   /* システムエラー状態 */
    long Detection;     /* 動物検知状態 */
} AnimalCameraState;

/*==============*/
/*  周辺カメラ  */
/*==============*/
/* 周辺カメラ 状態 */
typedef struct
{
    long ReceiveCount;  /* 受信回数 */
    long SystemError;   /* システムエラー状態 */
    long PersonDetect;  /* 人検知状態 */
} AroundCameraStateStr;


/*=================*/
/* 共有メモリ 本体 */
/*=================*/
typedef struct
{
    CommanderStateStr       Commander;      /* 司令塔マイコン 情報 */
    MotorStatusStr          Motor;          /* モータマイコン 情報 */
    AroundCameraStateStr    AroundCamera;   /* 周辺カメラマイコン 情報 */
    FrontCameraStateStr     FrontCamera;    /* 前方カメラマイコン 情報 */
    AnimalCameraState       AnimalCamera;   /* 動物カメラマイコン 情報 */
} ShareMemoryStr;

#ifdef MEMORY_MAIN
    ShareMemoryStr* pShareMemory;
#else
    extern ShareMemoryStr* pShareMemory;
#endif

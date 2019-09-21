#pragma once

/*----------------*/
/*  �ʒu���֘A  */
/*----------------*/
/* �r�[�R���� */
#define BEACON_COUNT    (3)

/* �����萔 (�l�͎��v���) */
typedef enum
{
    E_DIR_FRONT = 0,    /* 0: �O�i */
    E_DIR_FRONT_R,      /* 1: �E�΂ߑO */
    E_DIR_RIGHT,        /* 2: �E */
    E_DIR_BACK_R,       /* 3: �E�΂ߌ�� */
    E_DIR_BACK,         /* 4: ��� */
    E_DIR_BACK_L,       /* 5: ���΂ߌ�� */
    E_DIR_LEFT,         /* 6: �� */
    E_DIR_FRONT_L,      /* 7: ���΂ߑO */
    E_DIR_TYPE_MAX,     /* 8: ������ʐ� */
} DirectionEnum;

/* �d�g���x�֘A��� */
typedef struct
{
    float   RssiAverage;
    float   TxPowerAverage;
    float   Distance;
} BeaconDataStr;

/*------------------*/
/*  ���[�^�}�C�R��  */
/*------------------*/
/* ���[�^�}�C�R������ ����w�� */
typedef enum
{
    E_COMMAND_STOP = 0, /* 0: ��~ */
    E_COMMAND_FRONT,    /* 1: �O�i */
    E_COMMAND_U_TURN,   /* 2: U�^�[�� */
    E_COMMAND_AVOID,    /* 3: ��� */
    E_COMMAND_REMOTE,   /* 4: ���u���� */
    E_COMMAND_MONITOR,  /* 5: ���j�^ */
    E_COMMAND_MAX,      /* 6: �R�}���h�� */
} MotorCommandEnum;

/* ���[�^�}�C�R������ ������n����w�� */
typedef enum
{
    E_CUTTER_STOP = 0,  /* 0: ������n��~ */
    E_CUTTER_DRIVE,     /* 1: ������n�쓮 */
    E_CUTTER_TYPE_MAX,  /* 2: ������n ���䐔 */
} CutterDriveEnum;

/* ���u���샂�[�h */
typedef enum
{
    E_MODE_MANUAL = 0,  /* 0: �蓮 */
    E_MODE_AUTO,        /* 1: ���� */
} ControlModeEnum;

/* ���[�^�}�C�R�� ��� */
typedef struct
{
    MotorCommandEnum    Command;
    CutterDriveEnum     Cutter;
    long                PointX;
    long                PointY;
    long                ErrorStatus;
    ControlModeEnum     RemoteMode;
} MotorStatusStr;

/*--------------*/
/*  �O���J����  */
/*--------------*/
/* �O���J���� ��� */
typedef struct
{
    long ReceiveCount;  /* ��M�� */
    long SystemError;   /* �V�X�e���G���[��� */
    long Avoidance;     /* ����w�� */
} FrontCameraStateStr;

/*--------------*/
/*  ���ӃJ����  */
/*--------------*/
/* ���ӃJ���� ��� */
typedef struct
{
    long ReceiveCount;  /* ��M�� */
    long SystemError;   /* �V�X�e���G���[��� */
    long PersonDetect;  /* �l���m��� */
} AroundCameraStateStr;


/*=================*/
/* ���L������ �{�� */
/*=================*/
typedef struct
{
    BeaconDataStr           BeaconData[BEACON_COUNT];
    MotorStatusStr          MotorState;
    AroundCameraStateStr    AroundCamera;
    FrontCameraStateStr     FrontCamera;
} ShareMemoryStr;

#ifdef MEMORY_MAIN
    ShareMemoryStr* pShareMemory;
#else
    extern ShareMemoryStr* pShareMemory;
#endif

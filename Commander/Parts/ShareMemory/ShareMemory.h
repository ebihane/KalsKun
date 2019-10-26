#pragma once

/*========*/
/*  ����  */
/*========*/
/* ���m��Ԓ萔 */
typedef enum
{
    NOT_DETECT = 0,
    DETECTED,
} DetectTypeEnum;

/*==================*/
/*  �i�ߓ��}�C�R��  */
/*==================*/
/* �u�U�[�����[�h */
typedef enum
{
    E_MELODY_SILENT = 0,    /* 0: ��~ */
    E_MELODY_SOUND_1,       /* 1: �� 1 */
    E_MELODY_SOUND_2,       /* 2: �� 2 */
    E_MELODY_WARNING,       /* 3: �x�� */
    E_MELODY_ERROR,         /* 4: �ُ픭�� */
} MelodyModeEnum;

/* �w�������C�g���[�h */
typedef enum
{
    E_LIGHT_OFF = 0,        /* 0: OFF */
    E_LIGHT_ON,             /* 1: ON */
    E_LIGHT_BLINK,          /* 2: �_�� */
} LightModeEnum;

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

/*---------------------*/
/* �i�ߓ��}�C�R�� ��� */
/*---------------------*/
typedef struct
{
    BeaconDataStr   Beacon[BEACON_COUNT];
    MelodyModeEnum  MelodyMode;
    LightModeEnum   LightMode;
    long            ModeState;
    bool            SystemError;
} CommanderStateStr;

/*==================*/
/*  ���[�^�}�C�R��  */
/*==================*/
/* ���[�^�}�C�R������ ����w�� */
typedef enum
{
    E_COMMAND_STOP = 0, /* 0: ��~ */
    E_COMMAND_FRONT,    /* 1: �O�i */
    E_COMMAND_R_TURN,   /* 2: �E�^�[�� */
    E_COMMAND_AVOID,    /* 3: ��� */
    E_COMMAND_REMOTE,   /* 4: ���u���� */
    E_COMMAND_MONITOR,  /* 5: ���j�^ */
    E_COMMAND_L_TURN,   /* 6: ���^�[�� */
    E_COMMAND_MAX,      /* 7: �R�}���h�� */
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
    long                CommunicationCount;
    MotorCommandEnum    Command;
    CutterDriveEnum     Cutter;
    long                PointX;
    long                PointY;
    DetectTypeEnum      ErrorStatus;
    ControlModeEnum     RemoteMode;
    DetectTypeEnum      CommunicationError;
} MotorStatusStr;

/*==============*/
/*  �O���J����  */
/*==============*/
/* ����w���萔 */
typedef enum
{
    NOT_REQUEST = 0,    /* 0: �v������ */
    AVOIDANCE,          /* 1: ����w�� */
    TURN,               /* 2: �^�[���w�� */
} MoveTypeEnum;

/* �O���J���� ��� */
typedef struct
{
    long ReceiveCount;          /* ��M�� */
    long SystemError;           /* �V�X�e���G���[��� */
    MoveTypeEnum MoveType;      /* ����w�� */
    DetectTypeEnum RedTape;     /* �ԃe�[�v���m */
    DetectTypeEnum BlueObject;  /* ��Q�����m */
    float Distance[2];          /* ��Q���Ƃ̋��� */
} FrontCameraStateStr;

/*==============*/
/*  �����J����  */
/*==============*/
/* �����J���� ��� */
typedef struct
{
    long ReceiveCount;          /* ��M�� */
    long SystemError;           /* �V�X�e���G���[��� */
    DetectTypeEnum Human;       /* �l���m */
    DetectTypeEnum Animal;      /* �������m */
} AnimalCameraState;

/*==============*/
/*  ���ӃJ����  */
/*==============*/
/* ���ӃJ���� ��� */
typedef struct
{
    long ReceiveCount;          /* ��M�� */
    long SystemError;           /* �V�X�e���G���[��� */
    DetectTypeEnum Detect;      /* �ԊO�����m��� */
} AroundCameraStateStr;

/*=================*/
/* ���L������ �{�� */
/*=================*/
typedef struct
{
    CommanderStateStr       Commander;      /* �i�ߓ��}�C�R�� ��� */
    MotorStatusStr          Motor;          /* ���[�^�}�C�R�� ��� */
    AroundCameraStateStr    AroundCamera;   /* ���ӃJ�����}�C�R�� ��� */
    FrontCameraStateStr     FrontCamera;    /* �O���J�����}�C�R�� ��� */
    AnimalCameraState       AnimalCamera;   /* �����J�����}�C�R�� ��� */
} ShareMemoryStr;

#ifdef MEMORY_MAIN
    ShareMemoryStr* pShareMemory;
#else
    extern ShareMemoryStr* pShareMemory;
#endif

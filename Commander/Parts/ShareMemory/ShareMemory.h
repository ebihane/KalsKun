#pragma once

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

/* �i�ߓ��}�C�R�� �d�g���x�֘A��� */
typedef struct
{
    float   RssiAverage;
    float   TxPowerAverage;
    float   Distance;
} BeaconDataStr;


/* ���u����֘A */
typedef enum
{
    E_MODE_MANUAL = 0,  /* 0: �蓮 */
    E_MODE_AUTO,        /* 1: ���� */
} MoveTypeEnum;

/* �����R�� */
typedef struct
{
    MoveTypeEnum Mode;  /* ���䃂�[�h */

} MoveControlStr;






/* �O���J���� ��� */
typedef struct
{
    long ReceiveCount;  /* ��M�� */
    long SystemError;   /* �V�X�e���G���[��� */
    long Avoidance;     /* ����w�� */
} FrontCameraStateStr;

/* ���ӃJ���� ��� */
typedef struct
{
    long ReceiveCount;  /* ��M�� */
    long SystemError;   /* �V�X�e���G���[��� */
    long PersonDetect;  /* �l���m��� */
} AroundCameraStateStr;

/* ���[�^�}�C�R���ʐM �w�����e */
typedef struct
{
    char LeftRight;     /* ���E */
    char FrontBack;     /* �O�� */
    char CutterSpeed;   /* ������n�X�s�[�h */
} MotorCommandStr;




/*-----------------*/
/* ���L������ �{�� */
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

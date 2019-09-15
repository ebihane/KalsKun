#pragma once

/* ���p�萔 */
typedef enum
{
    E_COMPASS_NORTH = 0,    /* 0: �k */
    E_COMPASS_NORTHEAST,    /* 1: �k�� */
    E_COMPASS_EAST,         /* 2: �� */
    E_COMPASS_SOUTHEAST,    /* 3: �쓌 */
    E_COMPASS_SOUTH,        /* 4: �� */
    E_COMPASS_SOUTHWEST,    /* 5: �쐼 */
    E_COMPASS_WEST,         /* 6: �� */
    E_COMPASS_NORTHWEST,    /* 7: �k�� */
    E_COMPASS_MAX,          /* 8: ���p�� */
} CompassEnum;

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

/* �ʒu��� */
typedef struct
{
    unsigned long   CurrentX;   /* ���ݍ��W X */
    unsigned long   CurrentY;   /* ���ݍ��W Y */
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

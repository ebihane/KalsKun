#pragma once

/* ���p�萔 */
typedef enum
{
    E_COMPASS_NORTH = 0,    /* �k */
    E_COMPASS_NORTHEAST,    /* �k�� */
    E_COMPASS_EAST,         /* �� */
    E_COMPASS_SOUTHEAST,    /* �쓌 */
    E_COMPASS_SOUTH,        /* �� */
    E_COMPASS_SOUTHWEST,    /* �쐼 */
    E_COMPASS_WEST,         /* �� */
    E_COMPASS_NORTHWEST,    /* �k�� */
} CompassEnum;

/* �����萔 (�l�͎��v���) */
typedef enum
{
    E_DIR_FRONT = 0,    /* �O�i */
    E_DIR_FRONT_R,      /* �E�΂ߑO */
    E_DIR_RIGHT,        /* �E */
    E_DIR_BACK_R,       /* �E�΂ߌ�� */
    E_DIR_BACK,         /* ��� */
    E_DIR_BACK_L,       /* ���΂ߌ�� */
    E_DIR_LEFT,         /* �� */
    E_DIR_FRONT_L,      /* ���΂ߑO */
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

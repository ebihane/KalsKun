#pragma once

typedef struct
{
    long ReceiveCount;  /* ��M�� */
    long SystemError;   /* �V�X�e���G���[��� */
    long PersonDetect;  /* �l���m��� */
} AroundCameraStateStr;

typedef struct
{
    char LeftRight;     /* ���E */
    char FrontBack;     /* �O�� */
    char CutterSpeed;   /* ������n�X�s�[�h */
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

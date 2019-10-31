#pragma once

#include <wiringPi.h>

/* ���ʒ萔 */
typedef enum
{
    NormalEnd,          /* ����I�� */
    AbnormalEnd,        /* �ُ�I�� */
    Reconnect,          /* �Đڑ� (�ʐM�p) */
} ResultEnum;

/* �C�x���g�\���� */
typedef struct
{
    long Code;          /* �C�x���g�R�[�h */
    ResultEnum Result;  /* ���s���� */
    long lParam[5];     /* long �^�p�����[�^ */
    float fParam[10];   /* float �^�p�����[�^ */
} EventInfo;

/* ���W�\���� */
typedef struct
{
    long X;             /* �� */
    long Y;             /* �c */
} RectStr;

/* �T�C�Y�\���� */
typedef struct
{
    double Horizontal;  /* �� */
    double Vertical;    /* �c */
} SizeStr;

/* ���O�o�̓v���Z�X �֘A��` */
#define LOG_WRITER_QUEUE_NAME   "LogWriter"

#define LOG_EVCODE_OUTPUT       (1)     /* ���O�o�� */
#define LOG_EVCODE_STOP         (2)     /* ���O��~ */
#define LOG_EVCODE_RESTART      (3)     /* ���O�ĊJ */
#define LOG_EVCODE_FINISH       (99)    /* ���O�I�� */

/* IP �A�h���X��` (�L��) */
#define COMMANDER_IP_ADDRESS        "192.168.3.1"   /* �i�ߓ��}�C�R�� IP �A�h���X */
#define AROUND_CAMERA_IP_ADDRESS    "192.168.3.2"   /* 360���J�����}�C�R�� IP �A�h���X */
#define FRONT_CAMERA_IP_ADDRESS     "192.168.3.3"   /* �O���J�����}�C�R�� IP �A�h���X */

/* �|�[�g�ԍ���` (�L��) */
#define COMMANDER_TO_AC_PORT        (10001)         /* �i�ߓ��}�C�R�� �� 360���J�����}�C�R�� �ʐM�|�[�g�ԍ� */
#define COMMANDER_TO_FC_PORT        (10001)         /* �i�ߓ��}�C�R�� �� �O���J�����}�C�R�� �ʐM�|�[�g�ԍ� */
#define MOTOR_TO_COMMANDER_PORT     (10000)         /* ���[�^�}�C�R�� �� �i�ߓ��}�C�R�� �ʐM�X���b�h �|�[�g�ԍ� */
#define AC_TO_COMMANDER_PORT        (10001)         /* 360���J�����}�C�R�� �� �i�ߓ��}�C�R�� ��ԒʐM�X���b�h �|�[�g�ԍ� */
#define FC1_TO_COMMANDER_PORT       (10002)         /* �O���J����(�e�[�v���m�p)�}�C�R�� �� �i�ߓ��}�C�R�� ��ԒʐM�X���b�h �|�[�g�ԍ� */
#define FC2_TO_COMMANDER_PORT       (10003)         /* �O���J����(�猟�ؗp)�}�C�R�� �� �i�ߓ��}�C�R�� ��ԒʐM�X���b�h �|�[�g�ԍ� */

/* IP �A�h���X��` (Wifi) */
#define COMMANDER_WIFI_IP           "10.0.0.1"      /* �i�ߓ��}�C�R�� Wifi IP �A�h���X */
#define ACCESS_STATION_1_WIFI_IP    "10.0.0.11"     /* �A�N�Z�X�X�e�[�V����1 Wifi IP �A�h���X */
#define ACCESS_STATION_2_WIFI_IP    "10.0.0.12"     /* �A�N�Z�X�X�e�[�V����2 Wifi IP �A�h���X */
#define ACCESS_STATION_3_WIFI_IP    "10.0.0.13"     /* �A�N�Z�X�X�e�[�V����3 Wifi IP �A�h���X */
#define REMOCON_WIFI_IP             "10.0.0.101"    /* �����R���}�C�R�� Wifi IP �A�h���X */
#define TOOL_WIFI_IP                "10.0.0.102"    /* �ݒ�c�[�� PC Wifi IP �A�h���X */

/* �|�[�g�ԍ���` (Wifi) */
#define REMOCON_TO_COMMANDER_PORT   (20000)         /* �����R���}�C�R�� �� �i�ߓ��}�C�R�� �ʐM�X���b�h �|�[�g�ԍ� */
#define TOOL_TO_COMMANDER_PORT      (20001)         /* �ݒ�c�[�� �� �i�ߓ��}�C�R�� �ʐM�X���b�h �|�[�g�ԍ� */

#define LAST_TEST_PROC

#include <wiringPi.h>
#include <softTone.h>

/* #define AUTO_CHANGE_TRIGGER */

/* GPIO ��` */
#define GPIO_USE_PIN_COUNT  (13)
#define IO_SIMULATOR_ANIMAL (4)     /* IN  : �V�~�����[�V���� : �����J���� ���m */
#define IO_SHUTDOWN_1       (5)     /* IN  : �I���w�� 1 */  
#define IO_SHUTDOWN_2       (6)     /* IN  : �I���w�� 2 */
#define IO_KUSATORI_MODE    (16)    /* OUT : ������ */
#define IO_SIMULATOR_MOTOR  (17)    /* IN  : �V�~�����[�V���� : Motor Auto / Manual �؂�ւ�*/
#define IO_SIMULATOR_AVOID  (18)    /* IN  : �V�~�����[�V���� : �O���J���� ��Q�� */
#define IO_DIRECTION_LIGHT  (19)    /* OUT : �w�������C�g */
#define IO_YAKEI_MODE       (20)    /* OUT : ��x */
#define IO_FAILURE_STATE    (21)    /* OUT : �ُ픭���M�� */
#define IO_BUZZER           (23)    /* OUT : ���d�u�U�[ */
#define IO_SIMULATOR_AROUND (24)    /* IN  : �V�~�����[�V���� : ���ӃJ���� ���m */
#define IO_HEART_BEAT       (25)    /* OUT : Heart Beat */
#define IO_SIMULATOR_TAPE   (27)    /* IN  : �V�~�����[�V���� : �O���J���� �ԃe�[�v */

typedef struct
{
    int PinNo;
    int Mode;
} GpioInfoStr;

#ifdef MEMORY_MAIN
static const GpioInfoStr GPIO_INFO_TABLE[GPIO_USE_PIN_COUNT]
= {
    {   IO_SIMULATOR_ANIMAL,INPUT               },
    {   IO_SHUTDOWN_1,      INPUT               },
    {   IO_SHUTDOWN_2,      INPUT               },
    {   IO_KUSATORI_MODE,   OUTPUT              },
    {   IO_SIMULATOR_MOTOR, INPUT               },
    {   IO_SIMULATOR_AVOID, INPUT               },
    {   IO_DIRECTION_LIGHT, OUTPUT              },
    {   IO_YAKEI_MODE,      OUTPUT              },
    {   IO_FAILURE_STATE,   OUTPUT              },
    {   IO_BUZZER,          SOFT_TONE_OUTPUT    },
    {   IO_SIMULATOR_AROUND,INPUT               },
    {   IO_HEART_BEAT,      OUTPUT              },
    {   IO_SIMULATOR_TAPE,  INPUT               },
};

#else

extern const GpioInfoStr GPIO_INFO_TABLE[GPIO_USE_PIN_COUNT];

#endif
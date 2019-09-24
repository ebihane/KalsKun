#define LAST_TEST_PROC

#include <wiringPi.h>
#include <softTone.h>

/* GPIO ��` */
#define GPIO_USE_PIN_COUNT  (8)
#define IO_BUZZER           (23)    /* OUT : ���d�u�U�[ */
#define IO_SHUTDOWN_1       (5)     /* IN  : �I���w�� 1 */  
#define IO_SHUTDOWN_2       (6)     /* IN  : �I���w�� 2 */
#define IO_HEART_BEAT       (13)    /* OUT : Heart Beat */
#define IO_DIRECTION_LIGHT  (19)    /* OUT : �w�������C�g */
#define IO_KUSATORI_MODE    (16)    /* OUT : ������ */
#define IO_YAKEI_MODE       (20)    /* OUT : ��x */
#define IO_FAILURE_STATE    (21)    /* OUT : �ُ픭���M�� */

typedef struct
{
    int PinNo;
    int Mode;
} GpioInfoStr;

#ifdef MEMORY_MAIN
static const GpioInfoStr GPIO_INFO_TABLE[GPIO_USE_PIN_COUNT]
= {
    {   IO_SHUTDOWN_1,      INPUT               },
    {   IO_SHUTDOWN_2,      INPUT               },
    {   IO_HEART_BEAT,      OUTPUT              },
    {   IO_KUSATORI_MODE,   OUTPUT              },
    {   IO_DIRECTION_LIGHT, OUTPUT              },
    {   IO_YAKEI_MODE,      OUTPUT              },
    {   IO_FAILURE_STATE,   OUTPUT              },
    {   IO_BUZZER,          SOFT_TONE_OUTPUT    },
};

#else

extern const GpioInfoStr GPIO_INFO_TABLE[GPIO_USE_PIN_COUNT];

#endif
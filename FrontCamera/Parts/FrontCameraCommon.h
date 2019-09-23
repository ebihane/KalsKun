#define LAST_TEST_PROC

#include <wiringPi.h>

/* GPIO ��` */
#define GPIO_USE_PIN_COUNT  (9)
#define IO_SHUTDOWN_1       (5)     /* IN  : �I���w�� 1 */
#define IO_SHUTDOWN_2       (6)     /* IN  : �I���w�� 2 */
#define IO_TRIGGER1_PIN     (12)    /* OUT : �����g�Z���T1 Trigger Pin */
#define IO_HEART_BEAT       (13)    /* OUT : Heart Beat */
#define IO_ECHO1_PIN        (16)    /* IN  : �����g�Z���T1 Echo Pin */
#define IO_KUSATORI_MODE    (19)    /* OUT : ������ */
#define IO_TRIGGER2_PIN     (20)    /* OUT : �����g�Z���T2 Trigger Pin */
#define IO_ECHO2_PIN        (21)    /* IN  : �����g�Z���T2 Echo Pin */
#define IO_YAKEI_MODE       (26)    /* OUT : ��x */

typedef struct
{
    int PinNo;
    int Mode;
} GpioInfoStr;

#ifdef MEMORY_MAIN
static const GpioInfoStr GPIO_INFO_TABLE[GPIO_USE_PIN_COUNT]
= {
    {   IO_SHUTDOWN_1,      INPUT   },
    {   IO_SHUTDOWN_2,      INPUT   },
    {   IO_TRIGGER1_PIN,    OUTPUT  },
    {   IO_HEART_BEAT,      OUTPUT  },
    {   IO_ECHO1_PIN,       INPUT   },
    {   IO_KUSATORI_MODE,   OUTPUT  },
    {   IO_TRIGGER2_PIN,    OUTPUT  },
    {   IO_ECHO2_PIN,       INPUT   },
    {   IO_YAKEI_MODE,      OUTPUT  },
};

#else

extern const GpioInfoStr GPIO_INFO_TABLE[GPIO_USE_PIN_COUNT];

#endif
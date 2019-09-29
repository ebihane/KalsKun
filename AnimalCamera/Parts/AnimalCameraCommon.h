#define LAST_TEST_PROC

#include <wiringPi.h>

/* GPIO ��` */
#define GPIO_USE_PIN_COUNT  (6)
#define IO_SHUTDOWN_1       (5)     /* IN  : �I���w�� 1 */
#define IO_SHUTDOWN_2       (6)     /* IN  : �I���w�� 2 */
#define IO_HEART_BEAT       (13)    /* OUT : Heart Beat */
#define IO_KUSATORI_MODE    (19)    /* IN  : ������ */
#define IO_ERROR_LED        (23)    /* OUT : �ُ��� LED */
#define IO_YAKEI_MODE       (26)    /* IN  : ��x */

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
    {   IO_HEART_BEAT,      OUTPUT  },
    {   IO_KUSATORI_MODE,   INPUT   },
    {   IO_ERROR_LED,       OUTPUT  },
    {   IO_YAKEI_MODE,      INPUT   },
};

#else

extern const GpioInfoStr GPIO_INFO_TABLE[GPIO_USE_PIN_COUNT];

#endif
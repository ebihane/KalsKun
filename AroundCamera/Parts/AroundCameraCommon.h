#include <wiringPi.h>

/* GPIO ��` */
#define GPIO_USE_PIN_COUNT  (10)
#define IO_SHUTDOWN_1       (5)     /* IN  : �I���w�� 1 */  
#define IO_SHUTDOWN_2       (6)     /* IN  : �I���w�� 2 */
#define IO_REDWAVE_MON_1    (12)    /* IN  : �ԊO���Z���T���j�^ 1 */
#define IO_KUSAKARI_MODE    (19)    /* IN  : �����胂�[�h */
#define IO_REDWAVE_MON_2    (16)    /* IN  : �ԊO���Z���T���j�^ 2 */
#define IO_YAKEI_MODE       (26)    /* IN  : ��x���[�h */
#define IO_REDWAVE_MON_3    (20)    /* IN  : �ԊO���Z���T���j�^ 3 */
#define IO_REDWAVE_MON_4    (21)    /* IN  : �ԊO���Z���T���j�^ 4 */
#define IO_ERROR_LED        (23)    /* OUT : �ُ��� LED */
#define IO_HEART_BEAT       (25)    /* OUT : Heart Beat */

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
    {   IO_REDWAVE_MON_1,   INPUT   },
    {   IO_KUSAKARI_MODE,   INPUT   },
    {   IO_REDWAVE_MON_2,   INPUT   },
    {   IO_YAKEI_MODE,      INPUT   },
    {   IO_REDWAVE_MON_3,   INPUT   },
    {   IO_REDWAVE_MON_4,   INPUT   },
    {   IO_ERROR_LED,       OUTPUT  },
    {   IO_HEART_BEAT,      OUTPUT  },
};

#else

extern const GpioInfoStr GPIO_INFO_TABLE[GPIO_USE_PIN_COUNT];

#endif
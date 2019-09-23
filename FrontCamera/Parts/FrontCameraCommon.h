#define LAST_TEST_PROC

#include <wiringPi.h>

/* GPIO 定義 */
#define GPIO_USE_PIN_COUNT  (9)
#define IO_SHUTDOWN_1       (5)     /* IN  : 終了指示 1 */
#define IO_SHUTDOWN_2       (6)     /* IN  : 終了指示 2 */
#define IO_TRIGGER1_PIN     (12)    /* OUT : 超音波センサ1 Trigger Pin */
#define IO_HEART_BEAT       (13)    /* OUT : Heart Beat */
#define IO_ECHO1_PIN        (16)    /* IN  : 超音波センサ1 Echo Pin */
#define IO_KUSATORI_MODE    (19)    /* OUT : 草刈り */
#define IO_TRIGGER2_PIN     (20)    /* OUT : 超音波センサ2 Trigger Pin */
#define IO_ECHO2_PIN        (21)    /* IN  : 超音波センサ2 Echo Pin */
#define IO_YAKEI_MODE       (26)    /* OUT : 夜警 */

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
#define LAST_TEST_PROC

#include <wiringPi.h>

/* GPIO 定義 */
#define GPIO_USE_PIN_COUNT  (6)
#define IO_SHUTDOWN_1       (5)     /* IN  : 終了指示 1 */
#define IO_SHUTDOWN_2       (6)     /* IN  : 終了指示 2 */
#define IO_HEART_BEAT       (13)    /* OUT : Heart Beat */
#define IO_KUSATORI_MODE    (19)    /* IN  : 草刈り */
#define IO_ERROR_LED        (23)    /* OUT : 異常状態 LED */
#define IO_YAKEI_MODE       (26)    /* IN  : 夜警 */

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
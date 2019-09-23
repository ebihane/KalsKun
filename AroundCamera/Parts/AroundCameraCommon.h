#include <wiringPi.h>

/* GPIO 定義 */
#define GPIO_USE_PIN_COUNT  (9)
#define IO_SHUTDOWN_1       (5)     /* IN  : 終了指示 1 */  
#define IO_SHUTDOWN_2       (6)     /* IN  : 終了指示 2 */
#define IO_REDWAVE_MON_1    (12)    /* IN  : 赤外線センサモニタ 1 */
#define IO_HEART_BEAT       (13)    /* OUT : Heart Beat */
#define IO_KUSAKARI_MODE    (19)    /* IN  : 草刈りモード */
#define IO_REDWAVE_MON_2    (16)    /* IN  : 赤外線センサモニタ 2 */
#define IO_YAKEI_MODE       (26)    /* IN  : 夜警モード */
#define IO_REDWAVE_MON_3    (20)    /* IN  : 赤外線センサモニタ 3 */
#define IO_REDWAVE_MON_4    (21)    /* IN  : 赤外線センサモニタ 4 */

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
    {   IO_HEART_BEAT,      OUTPUT  },
    {   IO_KUSAKARI_MODE,   INPUT   },
    {   IO_REDWAVE_MON_2,   INPUT   },
    {   IO_YAKEI_MODE,      INPUT   },
    {   IO_REDWAVE_MON_3,   INPUT   },
    {   IO_REDWAVE_MON_4,   INPUT   },
};

#else

extern const GpioInfoStr GPIO_INFO_TABLE[GPIO_USE_PIN_COUNT];

#endif
#define LAST_TEST_PROC

#include <wiringPi.h>
#include <softTone.h>

/* GPIO 定義 */
#define GPIO_USE_PIN_COUNT  (8)
#define IO_BUZZER           (23)    /* OUT : 圧電ブザー */
#define IO_SHUTDOWN_1       (5)     /* IN  : 終了指示 1 */  
#define IO_SHUTDOWN_2       (6)     /* IN  : 終了指示 2 */
#define IO_HEART_BEAT       (13)    /* OUT : Heart Beat */
#define IO_DIRECTION_LIGHT  (19)    /* OUT : 指向性ライト */
#define IO_KUSATORI_MODE    (16)    /* OUT : 草刈り */
#define IO_YAKEI_MODE       (20)    /* OUT : 夜警 */
#define IO_FAILURE_STATE    (21)    /* OUT : 異常発生信号 */

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
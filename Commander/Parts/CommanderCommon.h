#define LAST_TEST_PROC

#include <wiringPi.h>
#include <softTone.h>

/* #define AUTO_CHANGE_TRIGGER */

/* GPIO 定義 */
#define GPIO_USE_PIN_COUNT  (13)
#define IO_SIMULATOR_ANIMAL (4)     /* IN  : シミュレーション : 動物カメラ 検知 */
#define IO_SHUTDOWN_1       (5)     /* IN  : 終了指示 1 */  
#define IO_SHUTDOWN_2       (6)     /* IN  : 終了指示 2 */
#define IO_KUSATORI_MODE    (16)    /* OUT : 草刈り */
#define IO_SIMULATOR_MOTOR  (17)    /* IN  : シミュレーション : Motor Auto / Manual 切り替え*/
#define IO_SIMULATOR_AVOID  (18)    /* IN  : シミュレーション : 前方カメラ 障害物 */
#define IO_DIRECTION_LIGHT  (19)    /* OUT : 指向性ライト */
#define IO_YAKEI_MODE       (20)    /* OUT : 夜警 */
#define IO_FAILURE_STATE    (21)    /* OUT : 異常発生信号 */
#define IO_BUZZER           (23)    /* OUT : 圧電ブザー */
#define IO_SIMULATOR_AROUND (24)    /* IN  : シミュレーション : 周辺カメラ 検知 */
#define IO_HEART_BEAT       (25)    /* OUT : Heart Beat */
#define IO_SIMULATOR_TAPE   (27)    /* IN  : シミュレーション : 前方カメラ 赤テープ */

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
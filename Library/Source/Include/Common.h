#pragma once

#include <wiringPi.h>

/* 結果定数 */
typedef enum
{
    NormalEnd,          /* 正常終了 */
    AbnormalEnd,        /* 異常終了 */
    Reconnect,          /* 再接続 (通信用) */
} ResultEnum;

/* イベント構造体 */
typedef struct
{
    long From;          /* 送信元 */
    long To;            /* 宛先 */
    long Code;          /* イベントコード */
    ResultEnum Result;  /* 実行結果 */
    long lParam[5];     /* long 型パラメータ */
    long fParam[10];    /* float 型パラメータ */
} EventInfo;

#define LOG_EVCODE_OUTPUT       (1)     /* ログ出力 */
#define LOG_EVCODE_STOP         (2)     /* ログ停止 */
#define LOG_EVCODE_RESTART      (3)     /* ログ再開 */

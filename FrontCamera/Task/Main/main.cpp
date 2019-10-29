#if 0
// Notify : Use OpenCV Version : 4.1.0-20190415.1
// Compile options (構成プロパティ - C/C++ - コマンドライン - 追加のオプション)
`pkg-config opencv --cflags` `pkg-config opencv --libs`
// Library Link (構成プロパティ - リンカー - 入力 - ライブラリの依存ファイル)
wiringPi; pthread; dl; rt;  opencv_core; opencv_video; opencv_videoio; opencv_highgui; opencv_imgproc; opencv_imgcodecs; Library
// Memo:
//  wifi アクセススキャン：sudo iwlist wlan0 scan
#endif

#define MEMORY_MAIN

#include <signal.h>

/* Common */
#include "Include/Common.h"
#include "Parts/FrontCameraCommon.h"

/* Parts */
#include "Logger/Logger.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Socket/TcpClient/TcpClient.h"

/* Task */
#include "Task/CameraCapture/CameraCapture.h"
#include "Task/StateSender/StateSender.h"
#include "Task/HeartBeat/HeartBeatManager.h"
#include "Task/Ultrasound/UltrasoundManager.h"
#include "Task/ErrorLed/ErrorLedManager.h"
#include "Task/ToolCommunicator/ToolCommunicator.h"

/* Parts */
static Logger* g_pLogger = NULL;

/* Task */
static CameraCapture* g_pCameraCapture = NULL;
static StateSender* g_pStateSender = NULL;
static HeartBeatManager* g_pHeartBeatManager = NULL;
static UltrasoundManager* g_pUltrasoundManager1 = NULL;
static UltrasoundManager* g_pUltrasoundManager2 = NULL;
static ErrorLedManager* g_pErrorLedManager = NULL;
static ToolCommunicator* g_pToolCommunicator = NULL;

ResultEnum initialize(const char controllerType);
void mainProcedure(const char isShow);
void finalize(const bool isShutdown);
void signalHandler(int signum);

/* ./FrontCamera.out (カメラ番号) (表示有無) */
int main(int argc, char* argv[])
{
    bool isMainLoopExit = false;
        
    char cameraNo = 0;
    char isShow = 0;

    signal(SIGKILL, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGABRT, signalHandler);

    if (2 <= argc)
    {
        cameraNo = (char)atoi(argv[1]);
    }

    if (3 <= argc)
    {
        isShow = (char)atoi(argv[2]);
    }

    if (initialize(cameraNo) != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    mainProcedure(isShow);

    isMainLoopExit = true;

FINISH:
    finalize(isMainLoopExit);

    if (isMainLoopExit == true)
    {
        system("sudo shutdown -h now &");
    }

    return 0;
}

ResultEnum initialize(const char cameraNo)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    TcpClient* client = NULL;

    /* I/O 初期化 */
    wiringPiSetupSys();
    {
        for (long lCnt = 0; lCnt < GPIO_USE_PIN_COUNT; lCnt++)
        {
            /* 入出力方向指定 */
            pinMode(GPIO_INFO_TABLE[lCnt].PinNo, GPIO_INFO_TABLE[lCnt].Mode);
        }
    }

    /* 共有メモリ インスタンス生成 */
    pShareMemory = new ShareMemoryStr();
    if (pShareMemory == NULL)
    {
        goto FINISH;
    }

    /* データ初期化 */
    pShareMemory->SystemError = false;
    pShareMemory->MoveType = MoveTypeEnum::NOT_REQUEST;
    pShareMemory->RedTape = DetectTypeEnum::NOT_DETECT;
    pShareMemory->BlueObject = DetectTypeEnum::NOT_DETECT;
    pShareMemory->UltrasoundData[0] = 999.9f;
    pShareMemory->UltrasoundData[1] = 999.9f;

    /* Log Process 起動 */
    StartLoggerProcess((char*)"FrontCamera");

    /* Log Process 生成待ち */
    delay(100);

    /* Log Accessor 生成 */
    g_pLogger = new Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::LogTypeEnum::BOTH_OUT);
    if (g_pLogger == NULL)
    {
        goto FINISH;
    }

    /* ハートビート制御スレッド 初期化 */
    g_pHeartBeatManager = new HeartBeatManager();
    if (g_pHeartBeatManager == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] HeartBeatManager allocation failed.\n");
        goto FINISH;
    }

    /* 超音波1 制御スレッド 初期化 */
    g_pUltrasoundManager1 = new UltrasoundManager(0);
    if (g_pUltrasoundManager1 == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] UltrasoundManager allocation failed.\n");
        goto FINISH;
    }

    /* 超音波2 制御スレッド 初期化 */
    g_pUltrasoundManager2 = new UltrasoundManager(1);
    if (g_pUltrasoundManager2 == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] UltrasoundManager allocation failed.\n");
        goto FINISH;
    }

    /* カメラ取得スレッド 起動 */
    g_pCameraCapture = new CameraCapture(cameraNo);
    if (g_pCameraCapture == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] g_pCameraCapture allocation failed.\n");
        goto FINISH;
    }

    /* 状態送信スレッド 起動 */
    client = new TcpClient((char*)COMMANDER_IP_ADDRESS, 10002);
    g_pStateSender = new StateSender(client);
    if (g_pStateSender == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] g_pStateSender allocation failed.\n");
        goto FINISH;
    }

    /* 異常状態 LED 制御スレッド 初期化 */
    g_pErrorLedManager = new ErrorLedManager();
    if (g_pErrorLedManager == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] g_pErrorLedManager allocation failed.\n");
        goto FINISH;
    }

    /* ツール通信スレッド 初期化 */
    g_pToolCommunicator = new ToolCommunicator();
    if (g_pToolCommunicator == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] g_pToolCommunicator allocation failed.\n");
        goto FINISH;
    }

    g_pHeartBeatManager->Run();
    g_pCameraCapture->Run();
    g_pUltrasoundManager1->Run();
    g_pUltrasoundManager2->Run();
    g_pStateSender->Run();
    g_pErrorLedManager->Run();
    g_pToolCommunicator->Run();

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

void mainProcedure(const char isShow)
{
    int key = 0;
    char logStr[80] = { 0 };
    Stopwatch watch;

    snprintf(&logStr[0], sizeof(logStr), "[mainProcedure] Main loop start. isShow[%d]\n", isShow);
    g_pLogger->LOG_INFO(logStr);

    watch.Start();
    while (1)
    {
        /* 赤テープ検知 */
        if (pShareMemory->RedTape != DetectTypeEnum::NOT_DETECT)
        {
            /* ターン指示 */
            pShareMemory->MoveType = MoveTypeEnum::TURN;
        }
        /* 青い物体があり、その距離が 40cm 以内 */
        else if ((pShareMemory->BlueObject != DetectTypeEnum::NOT_DETECT)
             &&  ((40 <= pShareMemory->UltrasoundData[0])
             ||   (40 <= pShareMemory->UltrasoundData[1])))
        {
            /* 回避指示 */
            pShareMemory->MoveType = MoveTypeEnum::AVOIDANCE;
        }
        /* 上記以外 */
        else
        {
            /* 要求無し */
            pShareMemory->MoveType = MoveTypeEnum::NOT_REQUEST;
        }

        if (isShow != 0)
        {
            if (g_pCameraCapture->IsCaptureStart() == true)
            {
                cv::imshow("camera", pShareMemory->Capture.Data[pShareMemory->Capture.Index]);
                key = cv::waitKey(1);
                if (key == 'q')
                {
                    break;
                }
            }
        }

        if (60.0f <= watch.GetSplit())
        {
            int shutdown1 = digitalRead(IO_SHUTDOWN_1);
            int shutdown2 = digitalRead(IO_SHUTDOWN_2);
            if ((shutdown1 == LOW) && (shutdown2 == LOW))
            {
                break;
            }
        }

        delay(10);
    }

    if (isShow != 0)
    {
        cv::destroyAllWindows();
    }
}

void finalize(const bool isShutdown)
{
    /* シャットダウン実施時はどうせシステム終了で落ちるので意図的な終了処理は無し */
    if (isShutdown == false)
    {
        if (g_pToolCommunicator != NULL)
        {
            g_pToolCommunicator->Stop(5);
            delete g_pToolCommunicator;
            g_pToolCommunicator = NULL;
        }

        if (g_pErrorLedManager != NULL)
        {
            g_pErrorLedManager->Stop(5);
            delete g_pErrorLedManager;
            g_pErrorLedManager = NULL;
        }

        if (g_pStateSender != NULL)
        {
            g_pStateSender->Stop(5);
            delete g_pStateSender;
            g_pStateSender = NULL;
        }

        if (g_pUltrasoundManager2 != NULL)
        {
            g_pUltrasoundManager2->Stop(5);
            delete g_pUltrasoundManager2;
            g_pUltrasoundManager2 = NULL;
        }

        if (g_pUltrasoundManager1 != NULL)
        {
            g_pUltrasoundManager1->Stop(5);
            delete g_pUltrasoundManager1;
            g_pUltrasoundManager1 = NULL;
        }

        if (g_pCameraCapture != NULL)
        {
            g_pCameraCapture->Stop(5);
            delete g_pCameraCapture;
            g_pCameraCapture = NULL;
        }

        if (g_pHeartBeatManager != NULL)
        {
            g_pHeartBeatManager->Stop(5);
            delete g_pHeartBeatManager;
            g_pHeartBeatManager = NULL;
        }

        if (g_pLogger != NULL)
        {
            delete g_pLogger;
            g_pLogger = NULL;
        }

        if (pShareMemory != NULL)
        {
            delete pShareMemory;
            pShareMemory = NULL;
        }

        StopLoggerProcess();
    }
}

void signalHandler(int signum)
{
    printf("ERR! [FrontCamera] Signal Catched. signum[%d]\n", signum);

    if (g_pLogger != NULL)
    {
        g_pLogger->LOG_ERROR("[FrontCamera] Signal Catched!!\n");
    }

    finalize(false);

    exit(0);
}

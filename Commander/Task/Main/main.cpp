#define MEMORY_MAIN

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "Parts/CommanderCommon.h"

/* Parts */
#include "Logger/Logger.h"
#include "Socket/TcpServer/TcpServer.h"
#include "Parts/Setting/SettingManager.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Parts/MappingData/AreaMap.h"
#include "Parts/MappingData/MoveMap.h"

/* Task */
#include "Task/AroundCamera/AroundCameraReceiver.h"
#include "Task/FrontCamera/FrontCameraReceiver.h"
#include "Task/MotorCommunicator/MotorCommunicator.h"
#include "Task/HeartBeat/HeartBeatManager.h"

/* Sequencer */
#include "Task/Main/Sequencer/SequencerBase.h"
#include "Task/Main/Sequencer/Idle/HimajinKun.h"
#include "Task/Main/Sequencer/Kusakari/KusakariKun.h"
#include "Task/Main/Sequencer/Yakei/YakeiKun.h"
#include "Task/Main/Sequencer/Error/ErrorKun.h"

/* Parts */
static Logger* g_pLogger = NULL;

/* Task */
static FrontCameraReceiver* g_pFrontCameraReceiver = NULL;
static AroundCameraReceiver* g_pAroundCameraReceiver = NULL;
static MotorCommunicator* g_pMotorCommunicator = NULL;
static HeartBeatManager* g_pHeartBeatManager = NULL;

/* Sequencer */
static SequencerBase* g_pSequencer = NULL;

ResultEnum initialize();
void mainProcedure();
void finalize();
void signalHandler(int signum);


int main(void)
{
    bool isMainLoopExit = false;

    signal(SIGKILL, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGABRT, signalHandler);

    if (initialize() != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    mainProcedure();
    isMainLoopExit = true;

FINISH :
    finalize();

    if (isMainLoopExit == true)
    {
        system("sudo shutdown -h now &");
    }

    return 0;
}

ResultEnum initialize()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    SettingManager* setting = NULL;
    TcpServer* server = NULL;
    AreaMap* areaMap = AreaMap::GetInstance();
    MoveMap* moveMap = MoveMap::GetInstance();
    long lCnt = 0;
    char command[32] = { 0 };

    /* I/O 初期化 */
    int result = wiringPiSetupSys();

    for (lCnt = 0; lCnt < GPIO_USE_PIN_COUNT; lCnt++)
    {
        /* 入出力方向指定 */
        /* /sys/class/gpio への export は、Shell を作成して対応 */
        /* → /usr/local/bin 下に gpio_init.sh を作成 */
        /*    /etc/rc.local に上記のスクリプト起動命令を記述 */
        if (GPIO_INFO_TABLE[lCnt].Mode == SOFT_TONE_OUTPUT)
        {
            /* nop. */
            /* スレッド起動時に Create する */
        }
        else
        {
            pinMode(GPIO_INFO_TABLE[lCnt].PinNo, GPIO_INFO_TABLE[lCnt].Mode);
        }
    }

    /* 共有メモリ インスタンス生成 */
    pShareMemory = new ShareMemoryStr();
    if (pShareMemory == NULL)
    {
        goto FINISH;
    }

    /* ログ関連 初期化 */
    g_pLogger = new Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::LogTypeEnum::BOTH_OUT);
    if (g_pLogger == NULL)
    {
        goto FINISH;
    }

    StartLoggerProcess((char*)"Commander");

    /* 設定 */
    setting = SettingManager::GetInstance();
    if (setting->IsFileExist() == false)
    {
        setting->CreateDefaultData();
    }
    else
    {
        setting->Load();
    }

    /* 範囲マップ */
    if (areaMap->IsFileExist() == true)
    {
        areaMap->Load();
    }
    else
    {
        areaMap->Allocate();
        areaMap->SetInitialData();
        areaMap->Save();
    }

    /* 動作マップ */
    if (moveMap->IsFileExist() == true)
    {
        moveMap->Load();
    }
    else
    {
        moveMap->Allocate();
        moveMap->SetInitialData();
        moveMap->Save();
    }

    snprintf(&command[0], sizeof(command), "wiringSetupSys Ret[%d]\n", result);
    g_pLogger->LOG_INFO(command);

    /* Task 生成 */
    /* ハートビート制御スレッド 初期化 */
    g_pHeartBeatManager = new HeartBeatManager();
    if (g_pHeartBeatManager == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] HeartBeatManager allocation failed.\n");
        goto FINISH;
    }

    /* 前方カメラマイコン状態取得スレッド 初期化 */
    server = new TcpServer(FC1_TO_COMMANDER_PORT);
    g_pFrontCameraReceiver = new FrontCameraReceiver(server);
    if (g_pFrontCameraReceiver == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] FrontCameraReceiver allocation failed.\n");
        goto FINISH;
    }

    /* 360°カメラマイコン状態取得スレッド 初期化 */
    server = new TcpServer(AC_TO_COMMANDER_PORT);
    g_pAroundCameraReceiver = new AroundCameraReceiver(server);
    if (g_pAroundCameraReceiver == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] AroundCameraReceiver allocation failed.\n");
        goto FINISH;
    }

    /* モータマイコン通信スレッド 初期化 */
    g_pMotorCommunicator = new MotorCommunicator();
    if (g_pMotorCommunicator == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] g_pMotorCommunicator allocation failed.\n");
        goto FINISH;
    }

    /* Task 起動 */
    g_pHeartBeatManager->Run();
    g_pFrontCameraReceiver->Run();
    g_pAroundCameraReceiver->Run();
    g_pMotorCommunicator->Run();

    /* Sequencer 生成 */
    /* 最初は Idle 状態 */
    g_pSequencer = new HimajinKun();
    if (g_pSequencer == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] Sequencer allocation failed.\n");
        goto FINISH;
    }

    g_pSequencer->Initialize(SequencerBase::SequenceTypeEnum::E_SEQ_IDLE);

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

void mainProcedure()
{
    SequencerBase::SequenceTypeEnum current = g_pSequencer->GetSequence();
    SequencerBase::SequenceTypeEnum next = SequencerBase::SequenceTypeEnum::E_SEQ_ERROR;
    Stopwatch watch;

    watch.Start();
    while (1)
    {
        current = g_pSequencer->GetSequence();
        next = g_pSequencer->Process();
        if (next != current)
        {
            g_pSequencer->Destroy();
            delete g_pSequencer;
            
            if (next == SequencerBase::SequenceTypeEnum::E_SEQ_IDLE)
            {
                g_pSequencer = new HimajinKun();
            }
            else if (next == SequencerBase::SequenceTypeEnum::E_SEQ_KUSAKARI)
            {
                g_pSequencer = new KusakariKun();
            }
            else if (next == SequencerBase::SequenceTypeEnum::E_SEQ_YAKEI)
            {
                g_pSequencer = new YakeiKun();
            }
            else
            {
                g_pSequencer = new ErrorKun();
            }

            g_pSequencer->Initialize(current);
        }

        if (60.0f <= watch.GetSplit())
        {
            int shutdown1 = digitalRead(IO_SHUTDOWN_1);
            int shutdown2 = digitalRead(IO_SHUTDOWN_2);
            if ((shutdown1 == LOW) && (shutdown2 == LOW))
            {
                g_pLogger->LOG_INFO("[mainProcedure] Shutdown!!!\n");
                break;
            }
        }

        delay(10);
    }
}

void finalize()
{
    if (g_pMotorCommunicator != NULL)
    {
        g_pMotorCommunicator->Stop(5);
        delete g_pMotorCommunicator;
        g_pMotorCommunicator = NULL;
    }

    if (g_pAroundCameraReceiver != NULL)
    {
        g_pAroundCameraReceiver->Stop(5);
        delete g_pAroundCameraReceiver;
        g_pAroundCameraReceiver = NULL;
    }

    if (g_pFrontCameraReceiver != NULL)
    {
        g_pFrontCameraReceiver->Stop(5);
        delete g_pFrontCameraReceiver;
        g_pFrontCameraReceiver = NULL;
    }

    MoveMap* moveMap = MoveMap::GetInstance();
    moveMap->Save();

    AreaMap* areaMap = AreaMap::GetInstance();
    areaMap->Save();

    /* ハートビート制御スレッドは最後に落とす */
    if (g_pHeartBeatManager != NULL)
    {
        g_pHeartBeatManager->Stop(5);
        delete g_pHeartBeatManager;
        g_pHeartBeatManager = NULL;
    }

    StopLoggerProcess();

    if (g_pLogger != NULL)
    {
        delete g_pLogger;
        g_pLogger = NULL;
    }
}

void signalHandler(int signum)
{
    printf("ERR! [LogWriter] Signal Catched. signum[%d]\n", signum);

    finalize();
    StopLoggerProcess();

    exit(0);
}

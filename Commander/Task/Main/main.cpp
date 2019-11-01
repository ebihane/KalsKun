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
#include "Parts/PositionConverter/Gyro/GyroConverter.h"
#include "Parts/MappingData/AreaMap.h"
#include "Parts/MappingData/MoveMap.h"
#include "Parts/PositionData/PositionData.h"

/* Simulator */
#include "Parts/Simulator/MotorSimulator.h"
#include "Parts/Simulator/FrontCameraSimulator.h"
#include "Parts/Simulator/AnimalCameraSimulator.h"
#include "Parts/Simulator/AroundCameraSimulator.h"

/* Task */
#include "Task/AroundCamera/AroundCameraReceiver.h"
#include "Task/FrontCamera/FrontCameraReceiver.h"
#include "Task/AnimalCamera/AnimalCameraReceiver.h"
#include "Task/MotorCommunicator/MotorCommunicator.h"
#include "Task/HeartBeat/HeartBeatManager.h"
#include "Task/BuzzerController/BuzzerController.h"
#include "Task/LightController/LightController.h"
#include "Task/Patrol/PatrolThread.h"
#include "Task/ToolCommunicator/ToolCommunicator.h"

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
static AnimalCameraReceiver* g_pAnimalCameraReceiver = NULL;
static MotorCommunicator* g_pMotorCommunicator = NULL;
static HeartBeatManager* g_pHeartBeatManager = NULL;
static BuzzerController* g_pBuzzerThread = NULL;
static LightController* g_pLightThread = NULL;
static PatrolThread* g_pPatrolThread = NULL;
static ToolCommunicator* g_pToolCommunicator = NULL;

/* Sequencer */
static SequencerBase* g_pSequencer = NULL;

ResultEnum initialize(const char isMotorSimulator, const char isFrontCameraSimulator, const char isAnimalCameraSimulator, const char isAroundCameraSimulator, const char isPatrolNotExist);
void mainProcedure();
void finalize(const bool isShutdown);
void signalHandler(int signum);


int main(int argc, char* argv[])
{
    bool isMainLoopExit = false;
    char isMotorSimulator = 0;
    char isFrontCameraSimulator = 0;
    char isAnimalCameraSimulator = 0;
    char isAroundCameraSimulator = 0;
    char isPatrolNotExist = 0;

    if (2 <= argc)
    {
        isMotorSimulator = (char)atoi(argv[1]);
    }

    if (3 <= argc)
    {
        isFrontCameraSimulator = (char)atoi(argv[2]);
    }

    if (4 <= argc)
    {
        isAnimalCameraSimulator = (char)atoi(argv[3]);
    }

    if (5 <= argc)
    {
        isAroundCameraSimulator = (char)atoi(argv[4]);
    }

    if (6 <= argc)
    {
        isPatrolNotExist = (char)atoi(argv[5]);
    }

    signal(SIGKILL, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGABRT, signalHandler);

    if (initialize(isMotorSimulator, isFrontCameraSimulator, isAnimalCameraSimulator, isAroundCameraSimulator, isPatrolNotExist) != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    mainProcedure();
    isMainLoopExit = true;

FINISH :
    finalize(isMainLoopExit);

    if (isMainLoopExit == true)
    {
        system("sudo shutdown -h now &");
    }

    return 0;
}

ResultEnum initialize(const char isMotorSimulator, const char isFrontCameraSimulator, const char isAnimalCameraSimulator, const char isAroundCameraSimulator, const char isPatrolNotExist)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    SettingManager* setting = NULL;
    AdapterBase* adapter = NULL;
    GyroConverter* converter = NULL;
    Serial::SerialInfoStr serialSetting;
    AreaMap* areaMap = AreaMap::GetInstance();
    MoveMap* moveMap = MoveMap::GetInstance();
    PositionData* positionData = PositionData::GetInstance();
    long lCnt = 0;

    /* I/O 初期化 */
    wiringPiSetupSys();

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

    /* 司令塔状態 初期化 */
    pShareMemory->Commander.SystemError = false;
    pShareMemory->Commander.LightMode = LightModeEnum::E_LIGHT_OFF;
    pShareMemory->Commander.MelodyMode = MelodyModeEnum::E_MELODY_SILENT;

    /* モータマイコン状態 初期化 */
    pShareMemory->Motor.CommunicationCount = 0;
    pShareMemory->Motor.Command = MotorCommandEnum::E_COMMAND_STOP;
    pShareMemory->Motor.Cutter = CutterDriveEnum::E_CUTTER_STOP;
    pShareMemory->Motor.GyroX = 0;
    pShareMemory->Motor.GyroY = 0;
    pShareMemory->Motor.PointX = 0;
    pShareMemory->Motor.PointY = 0;
    pShareMemory->Motor.ErrorStatus = DetectTypeEnum::NOT_DETECT;
    pShareMemory->Motor.RemoteMode = ControlModeEnum::E_MODE_MANUAL;

    /* 前方カメラマイコン状態 初期化 */
    pShareMemory->FrontCamera.ReceiveCount = 0;
    pShareMemory->FrontCamera.SystemError = 0;
    pShareMemory->FrontCamera.MoveType = MoveTypeEnum::NOT_REQUEST;
    pShareMemory->FrontCamera.RedTape = DetectTypeEnum::NOT_DETECT;
    pShareMemory->FrontCamera.BlueObject = DetectTypeEnum::NOT_DETECT;

    /* 動物カメラ状態 初期化 */
    pShareMemory->AnimalCamera.ReceiveCount = 0;
    pShareMemory->AnimalCamera.SystemError = 0;
    pShareMemory->AnimalCamera.Human = DetectTypeEnum::NOT_DETECT;
    pShareMemory->AnimalCamera.Animal = DetectTypeEnum::NOT_DETECT;

    /* 周辺カメラ状態 初期化 */
    pShareMemory->AroundCamera.ReceiveCount = 0;
    pShareMemory->AroundCamera.SystemError = 0;
    pShareMemory->AroundCamera.Detect = DetectTypeEnum::NOT_DETECT;

    /* Log Process 起動 */
    StartLoggerProcess((char*)"Commander");

    /* Log Process 起動完了待ち */
    delay(100);

    /* Log Accessor 生成 */
    g_pLogger = new Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::LogTypeEnum::BOTH_OUT);
    if (g_pLogger == NULL)
    {
        goto FINISH;
    }

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

    /* 位置情報 */
    if (positionData->IsFileExist() == true)
    {
        positionData->Load();
    }
    else
    {
        positionData->SetInitialData();
        positionData->Save();
    }

    /* Task 生成 */
    /* ハートビート制御スレッド 初期化 */
    g_pHeartBeatManager = new HeartBeatManager();
    if (g_pHeartBeatManager == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] HeartBeatManager allocation failed.\n");
        goto FINISH;
    }

    if (isFrontCameraSimulator == 1)
    {
        /* 前方カメラシミュレータ */
        adapter = new FrontCameraSimulator();
    }
    else
    {
        /* TCP Server */
        adapter = new TcpServer(FC1_TO_COMMANDER_PORT);
    }

    /* 前方カメラマイコン状態取得スレッド 初期化 */
    g_pFrontCameraReceiver = new FrontCameraReceiver(adapter);
    if (g_pFrontCameraReceiver == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] FrontCameraReceiver allocation failed.\n");
        goto FINISH;
    }

    if (isAnimalCameraSimulator == 1)
    {
        /* 動物カメラシミュレータ */
        adapter = new AnimalCameraSimulator();
    }
    else
    {
        /* TCP Server */
        adapter = new TcpServer(FC2_TO_COMMANDER_PORT);
    }

    /* 動物カメラマイコン状態取得スレッド 初期化 */
    g_pAnimalCameraReceiver = new AnimalCameraReceiver(adapter);
    if (g_pAnimalCameraReceiver == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] AnimalCameraReceiver allocation failed.\n");
        goto FINISH;
    }

    if (isAroundCameraSimulator == 1)
    {
        /* 周辺カメラシミュレータ */
        adapter = new AroundCameraSimulator();
    }
    else
    {
        /* TCP Server */
        adapter = new TcpServer(AC_TO_COMMANDER_PORT);
    }

    /* 360°カメラマイコン状態取得スレッド 初期化 */
    g_pAroundCameraReceiver = new AroundCameraReceiver(adapter);
    if (g_pAroundCameraReceiver == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] AroundCameraReceiver allocation failed.\n");
        goto FINISH;
    }

    /* ブザー吹鳴スレッド生成 */
    g_pBuzzerThread = new BuzzerController();
    if (g_pBuzzerThread == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] BuzzerThread allocation failed.\n");
        goto FINISH;
    }

    /* 指向性ライト制御スレッド生成 */
    g_pLightThread = new LightController();
    if (g_pLightThread == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] LightThread allocation failed.\n");
        goto FINISH;
    }

    if (isMotorSimulator == 1)
    {
        /* モータシミュレータ */
        adapter = new MotorSimulator();
    }
    else
    {
        /* シリアルポート設定 */
        serialSetting.Baudrate = Serial::BaudrateEnum::E_Baudrate_115200;
        serialSetting.Parity = Serial::ParityEnum::E_Parity_Non;
        serialSetting.StopBit = Serial::StopBitEnum::E_StopBit_1Bit;
        serialSetting.DataLength = Serial::DataLengthEnum::E_Data_8bit;

        /* シリアル通信クラス生成 */
        adapter = new Serial((char*)"ttyUSB0", &serialSetting);
        if (adapter == NULL)
        {
            g_pLogger->LOG_ERROR("[initialize] Serial allocation failed.\n");
            goto FINISH;
        }
    }

    /* モータマイコン通信スレッド 初期化 */
    converter = new GyroConverter();
    g_pMotorCommunicator = new MotorCommunicator(adapter, converter);
    if (g_pMotorCommunicator == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] g_pMotorCommunicator allocation failed.\n");
        goto FINISH;
    }

    /* ツール通信スレッド 初期化 */
    g_pToolCommunicator = new ToolCommunicator();
    if (g_pToolCommunicator == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] g_pToolCommunicator allocation failed.\n");
        goto FINISH;
    }

    /* Task 起動 */
    g_pHeartBeatManager->Run();
    g_pFrontCameraReceiver->Run();
    g_pAnimalCameraReceiver->Run();
    g_pAroundCameraReceiver->Run();
    g_pBuzzerThread->Run();
    g_pLightThread->Run();
    g_pMotorCommunicator->Run();
    g_pToolCommunicator->Run();

    if (isPatrolNotExist != 1)
    {
        /* パトロールスレッド 初期化 */
        g_pPatrolThread = new PatrolThread();
        if (g_pPatrolThread == NULL)
        {
            g_pLogger->LOG_ERROR("[initialize] g_pPatrolThread allocation failed.\n");
            goto FINISH;
        }

        /* パトロール対象スレッドを登録 */
        g_pPatrolThread->AddTarget(g_pHeartBeatManager);
        g_pPatrolThread->AddTarget(g_pFrontCameraReceiver);
        g_pPatrolThread->AddTarget(g_pAnimalCameraReceiver);
        g_pPatrolThread->AddTarget(g_pAroundCameraReceiver);
        g_pPatrolThread->AddTarget(g_pBuzzerThread);
        g_pPatrolThread->AddTarget(g_pLightThread);
        g_pPatrolThread->AddTarget(g_pMotorCommunicator);

        g_pPatrolThread->Run();
    }

    /* エラー状態出力を OFF */
    digitalWrite(IO_FAILURE_STATE, LOW);

    /* Sequencer 生成 */
    /* 最初は Idle 状態 */
    g_pSequencer = new HimajinKun();
    if (g_pSequencer == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] Sequencer allocation failed.\n");
        goto FINISH;
    }

    g_pSequencer->Initialize(SequencerBase::SequenceTypeEnum::E_SEQ_IDLE);
    pShareMemory->Commander.ModeState = g_pSequencer->GetSequence();

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

void mainProcedure()
{
    SequencerBase::SequenceTypeEnum current = g_pSequencer->GetSequence();
    SequencerBase::SequenceTypeEnum next = SequencerBase::SequenceTypeEnum::E_SEQ_ERROR;
    Stopwatch watch;
    char logStr[64] = { 0 };

    g_pLogger->LOG_INFO("[mainProcedure] Main loop enter.\n");
    watch.Start();
    while (1)
    {
        current = g_pSequencer->GetSequence();
        pShareMemory->Commander.ModeState = current;
        next = g_pSequencer->Process();
        if (next != current)
        {
            snprintf(&logStr[0], sizeof(logStr), "[mainProcedure] Sequence Change. [%d -> %d]\n", current, next);
            g_pLogger->LOG_INFO(logStr);

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

void finalize(const bool isShutdown)
{
    MoveMap* moveMap = MoveMap::GetInstance();
    moveMap->Save();

    AreaMap* areaMap = AreaMap::GetInstance();
    areaMap->Save();

    PositionData* position = PositionData::GetInstance();
    position->Save();

    /* シャットダウン実施時はどうせシステム終了で落ちるので意図的な終了処理は無し */
    if (isShutdown == false)
    {
        if (g_pPatrolThread != NULL)
        {
            g_pPatrolThread->Stop(5);
            delete g_pPatrolThread;
            g_pPatrolThread = NULL;
        }

        if (g_pToolCommunicator != NULL)
        {
            g_pToolCommunicator->Stop(5);
            delete g_pToolCommunicator;
            g_pToolCommunicator = NULL;
        }

        if (g_pMotorCommunicator != NULL)
        {
            g_pMotorCommunicator->Stop(5);
            delete g_pMotorCommunicator;
            g_pMotorCommunicator = NULL;
        }

        if (g_pLightThread != NULL)
        {
            g_pLightThread->Stop(5);
            delete g_pLightThread;
            g_pLightThread = NULL;
        }

        if (g_pBuzzerThread != NULL)
        {
            g_pBuzzerThread->Stop(5);
            delete g_pBuzzerThread;
            g_pBuzzerThread = NULL;
        }

        if (g_pAroundCameraReceiver != NULL)
        {
            g_pAroundCameraReceiver->Stop(5);
            delete g_pAroundCameraReceiver;
            g_pAroundCameraReceiver = NULL;
        }

        if (g_pAnimalCameraReceiver != NULL)
        {
            g_pAnimalCameraReceiver->Stop(5);
            delete g_pAnimalCameraReceiver;
            g_pAnimalCameraReceiver = NULL;
        }

        if (g_pFrontCameraReceiver != NULL)
        {
            g_pFrontCameraReceiver->Stop(5);
            delete g_pFrontCameraReceiver;
            g_pFrontCameraReceiver = NULL;
        }

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
}

void signalHandler(int signum)
{
    printf("ERR! [Commander] Signal Catched. signum[%d]\n", signum);

    if (g_pLogger != NULL)
    {
        g_pLogger->LOG_ERROR("[Commander] Signal Catched!!\n");
    }

    finalize(false);

    exit(0);
}

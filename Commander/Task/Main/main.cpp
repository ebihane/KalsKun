#define MEMORY_MAIN

#include <stdlib.h>

/* Parts */
#include "Logger/Logger.h"
#include "Queue/Queue.h"
#include "Socket/TcpServer/TcpServer.h"
#include "Parts/Setting/SettingManager.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Parts/MappingData/AreaMap.h"
#include "Parts/MappingData/MoveMap.h"

/* Task */
#include "Task/AroundCamera/AroundCameraReceiver.h"
#include "Task/FrontCamera/FrontCameraReceiver.h"
#include "Task/MotorCommunicator/MotorCommunicator.h"

/* Sequencer */
#include "Task/Main/Sequencer/SequencerBase.h"
#include "Task/Main/Sequencer/Idle/HimajinKun.h"
#include "Task/Main/Sequencer/Kusakari/KusakariKun.h"
#include "Task/Main/Sequencer/Yakei/YakeiKun.h"
#include "Task/Main/Sequencer/Setting/SetteiKun.h"
#include "Task/Main/Sequencer/Error/ErrorKun.h"

/* Parts */
static Logger* g_pLogger = NULL;

/* Task */
static FrontCameraReceiver* g_pFrontCameraReceiver = NULL;
static AroundCameraReceiver* g_pAroundCameraReceiver = NULL;
static MotorCommunicator* g_pMotorCommunicator = NULL;

/* Sequencer */
static SequencerBase* g_pSequencer = NULL;

ResultEnum initialize();
void mainProcedure();
void finalize();

int main(void)
{
    bool isMainLoopExit = false;

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


    wiringPiSetupSys();

    /* 共有メモリ インスタンス生成 */
    pShareMemory = new ShareMemoryStr();
    if (pShareMemory == NULL)
    {
        goto FINISH;
    }

    /* ログ関連 初期化 */
    StartLoggerProcess((char*)"Commander");
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

    /* Task 生成 */
    /* 前方カメラマイコン状態取得スレッド 初期化 */
    server = new TcpServer(FC_TO_COMMANDER_PORT);
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
    g_pFrontCameraReceiver->Run();
    g_pAroundCameraReceiver->Run();
    g_pMotorCommunicator->Run();

    /* Sequencer 生成 */
    /* 最初は Idle 状態 */
    g_pSequencer = new HimajinKun();

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

void mainProcedure()
{
    SequencerBase::SequenceTypeEnum next = SequencerBase::SequenceTypeEnum::E_SEQ_ERROR;

    while (1)
    {
        next = g_pSequencer->Process();
        if (next != g_pSequencer->GetSequence())
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
            else if (next == SequencerBase::SequenceTypeEnum::E_SEQ_SETTING)
            {
                g_pSequencer = new SetteiKun();
            }
            else
            {
                g_pSequencer = new ErrorKun();
            }

            g_pSequencer->Initialize();
        }

        delay(10);
    }
}

void finalize()
{
    if (g_pMotorCommunicator != NULL)
    {
        g_pMotorCommunicator->Stop(10000);
        delete g_pMotorCommunicator;
        g_pMotorCommunicator = NULL;
    }

    if (g_pAroundCameraReceiver != NULL)
    {
        g_pAroundCameraReceiver->Stop(10000);
        delete g_pAroundCameraReceiver;
        g_pAroundCameraReceiver = NULL;
    }

    if (g_pFrontCameraReceiver != NULL)
    {
        g_pFrontCameraReceiver->Stop(10000);
        delete g_pFrontCameraReceiver;
        g_pFrontCameraReceiver = NULL;
    }

    MoveMap* moveMap = MoveMap::GetInstance();
    moveMap->Save();

    AreaMap* areaMap = AreaMap::GetInstance();
    areaMap->Save();

    if (g_pLogger != NULL)
    {
        delete g_pLogger;
        g_pLogger = NULL;
    }
}
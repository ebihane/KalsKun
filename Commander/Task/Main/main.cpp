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
static AreaMap* g_pAreaMap = NULL;
static MoveMap* g_pMoveMap = NULL;

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
    if (initialize() != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    mainProcedure();

FINISH :
    finalize();
    return 0;
}

ResultEnum initialize()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    SettingManager* setting = NULL;
    TcpServer* server = NULL;
    Queue* queue = NULL;

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
    g_pAreaMap = new AreaMap();
    if (g_pAreaMap == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] AreaMap allocation failed.\n");
        goto FINISH;
    }

    if (g_pAreaMap->IsFileExist() == true)
    {
        g_pAreaMap->Load();
    }
    else
    {
        g_pAreaMap->Allocate();
        g_pAreaMap->SetInitialData();
        g_pAreaMap->Save();
    }

    /* 動作マップ */
    g_pMoveMap = new MoveMap();
    if (g_pMoveMap == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] MoveMap allocation failed.\n");
        goto FINISH;
    }

    if (g_pMoveMap->IsFileExist() == true)
    {
        g_pMoveMap->Load();
    }
    else
    {
        g_pMoveMap->Allocate();
        g_pMoveMap->SetInitialData();
        g_pMoveMap->Save();
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
    queue = new Queue((char*)"MotorComm");
    g_pMotorCommunicator = new MotorCommunicator(queue);
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

    if (g_pMoveMap != NULL)
    {
        g_pMoveMap->Save();
        delete g_pMoveMap;
        g_pMoveMap = NULL;
    }

    if (g_pAreaMap != NULL)
    {
        g_pAreaMap->Save();
        delete g_pAreaMap;
        g_pAreaMap = NULL;
    }

    if (g_pLogger != NULL)
    {
        delete g_pLogger;
        g_pLogger = NULL;
    }
}
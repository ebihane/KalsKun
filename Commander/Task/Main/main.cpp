#define MEMORY_MAIN

#include <stdlib.h>
#include "Logger/Logger.h"
#include "Socket/TcpServer/TcpServer.h"
#include "Serial/Serial.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Task/AroundCamera/AroundCameraReceiver.h"
#include "Task/MotorCommunicator/MotorCommunicator.h"

#include "Task/Main/Sequencer/SequencerBase.h"
#include "Task/Main/Sequencer/Idle/HimajinKun.h"
#include "Task/Main/Sequencer/Kusakari/KusakariKun.h"
#include "Task/Main/Sequencer/Yakei/YakeiKun.h"
#include "Task/Main/Sequencer/Setting/SetteiKun.h"
#include "Task/Main/Sequencer/Error/ErrorKun.h"

static Logger* g_pLogger = NULL;
static AroundCameraReceiver* g_pAroundCameraReceiver = NULL;
static MotorCommunicator* g_pMotorCommunicator = NULL;

/* シーケンス：State Pattern */
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
    TcpServer* server = NULL;
    Serial* serial = NULL;
    Serial::SerialInfoStr serialSetting;

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

    /* 360°カメラマイコン状態取得スレッド 初期化 */
    server = new TcpServer(AC_TO_COMMANDER_PORT);
    g_pAroundCameraReceiver = new AroundCameraReceiver(server);
    if (g_pAroundCameraReceiver == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] FrontCameraSender allocation failed.\n");
        goto FINISH;
    }

    /* モータマイコン通信スレッド 初期化 */
    serialSetting.Baudrate = Serial::BaudrateEnum::E_Baudrate_115200;
    serialSetting.Parity = Serial::ParityEnum::E_Parity_Non;
    serialSetting.StopBit = Serial::StopBitEnum::E_StopBit_1Bit;
    serialSetting.DataLength = Serial::DataLengthEnum::E_Data_8bit;

    serial = new Serial((char *)"ttyUSB0", &serialSetting);
    g_pMotorCommunicator = new MotorCommunicator(serial);
    if (g_pMotorCommunicator == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] g_pMotorCommunicator allocation failed.\n");
        goto FINISH;
    }

    g_pAroundCameraReceiver->Run();
//    g_pMotorCommunicator->Run();

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

    if (g_pLogger != NULL)
    {
        delete g_pLogger;
        g_pLogger = NULL;
    }
}
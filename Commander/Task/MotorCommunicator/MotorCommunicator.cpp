#include <stdio.h>
#include <stdlib.h>
#include "MotorCommunicator.h"

MotorCommunicator::MotorCommunicator()
 : LoopThreadBase(100, TypeEnum::TIMER_STOP)
 , m_Queue(NULL)
 , m_Serial(NULL)
 , m_MotorCommand(MotorCommandEnum::E_COMMAND_STOP)
 , m_CutterMode(CutterDriveEnum::E_CUTTER_STOP)
{
    /* nop. */
}

MotorCommunicator::~MotorCommunicator()
{
    /* nop. */
}

ResultEnum MotorCommunicator::initializeCore()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    Serial::SerialInfoStr serialSetting;
    Queue* queue = NULL;
    Serial* serial = NULL;
    char sendBuffer[3] = { 0 };
    char recvBuffer[8] = { 0 };


    /* 受信キュー生成 */
    queue = new Queue((char*)"MotorComm");
    if (queue == NULL)
    {
        m_Logger->LOG_ERROR("[initializeCore] Queue allocation failed.\n");
        goto FINISH;
    }

    /* キューオープン */
    if (queue->Open() != ResultEnum::NormalEnd)
    {
        m_Logger->LOG_ERROR("[initializeCore] Queue Open failed.\n");
        goto FINISH;
    }

    /* シリアルポート設定 */
    serialSetting.Baudrate = Serial::BaudrateEnum::E_Baudrate_115200;
    serialSetting.Parity = Serial::ParityEnum::E_Parity_Non;
    serialSetting.StopBit = Serial::StopBitEnum::E_StopBit_1Bit;
    serialSetting.DataLength = Serial::DataLengthEnum::E_Data_8bit;

    /* シリアル通信クラス生成 */
    serial = new Serial((char*)"ttyUSB0", &serialSetting);
    if (serial == NULL)
    {
        m_Logger->LOG_ERROR("[initializeCore] Serial allocation failed.\n");
        goto FINISH;
    }

    /* ポートオープン */
    if (serial->Open() != ResultEnum::NormalEnd)
    {
        m_Logger->LOG_ERROR("[initializeCore] Serial Open failed.\n");
        goto FINISH;
    }

    /* お試しワンショット送信 */
    createSendData(MotorCommandEnum::E_COMMAND_STOP, CutterDriveEnum::E_CUTTER_STOP, &sendBuffer[0]);
    if (m_Serial->Send(&sendBuffer[0], sizeof(sendBuffer)) != ResultEnum::NormalEnd)
    {
        char log[40] = { 0 };
        snprintf(&log[0], sizeof(log), "[doMainProc] Send failed. errno[%d]\n", m_Serial->GetLastError());
        m_Logger->LOG_ERROR(log);
        goto FINISH;
    }

    if (m_Serial->Receive(&recvBuffer[0], sizeof(recvBuffer)) != ResultEnum::NormalEnd)
    {
        char log[40] = { 0 };
        snprintf(&log[0], sizeof(log), "[doMainProc] Receive failed. errno[%d]\n", m_Serial->GetLastError());
        m_Logger->LOG_ERROR(log);
        goto FINISH;
    }

    analyze(&recvBuffer[0]);

    m_Queue = queue;
    m_Serial = serial;

    retVal = ResultEnum::NormalEnd;

FINISH:

    if (retVal != ResultEnum::NormalEnd)
    {
        if (serial != NULL)
        {
            serial->Close();
            delete serial;
        }

        if (queue != NULL)
        {
            queue->Close();
            delete queue;
        }
    }

    return retVal;
}

ResultEnum MotorCommunicator::doMainProc()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    char sendBuffer[3] = { 0 };
    char recvBuffer[8] = { 0 };
    bool receivable = false;
    EventInfo ev = { 0 };
    
    if (m_Queue->IsReceivable(receivable) != ResultEnum::NormalEnd)
    {
        char log[64] = { 0 };
        snprintf(&log[0], sizeof(log), "[doMainProc] Queue::IsReceivable failed. errno[%d]\n", m_Queue->GetLastError());
        m_Logger->LOG_ERROR(log);
        goto FINISH;
    }

    if (receivable == true)
    {
        switch (ev.Code)
        {
            case 0 :    /* 0: 動作変更 */
                m_MotorCommand = (MotorCommandEnum)ev.lParam[0];
                break;
                
            case 1 :    /* 1: 草刈り刃動作変更 */
                m_CutterMode = (CutterDriveEnum)ev.lParam[0];
                break;

            case 2 :    /* 2: 動作・草刈り刃 同時変更 */
                m_MotorCommand = (MotorCommandEnum)ev.lParam[0];
                m_CutterMode = (CutterDriveEnum)ev.lParam[1];
                break;

            default :
                goto FINISH;
        }

        createSendData(m_MotorCommand, m_CutterMode, &sendBuffer[0]);
    }
    else
    {
        createSendData(MotorCommandEnum::E_COMMAND_MONITOR, m_CutterMode, &sendBuffer[0]);
    }

    if (m_Serial->Send(&sendBuffer[0], sizeof(sendBuffer)) != ResultEnum::NormalEnd)
    {
        char log[40] = { 0 };
        snprintf(&log[0], sizeof(log), "[doMainProc] Send failed. errno[%d]\n", m_Serial->GetLastError());
        m_Logger->LOG_ERROR(log);
        goto FINISH;
    }

    if (m_Serial->Receive(&recvBuffer[0], sizeof(recvBuffer)) != ResultEnum::NormalEnd)
    {
        char log[40] = { 0 };
        snprintf(&log[0], sizeof(log), "[doMainProc] Receive failed. errno[%d]\n", m_Serial->GetLastError());
        m_Logger->LOG_ERROR(log);
        goto FINISH;
    }

    analyze(&recvBuffer[0]);

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

ResultEnum MotorCommunicator::finalizeCore()
{
    if (m_Serial != NULL)
    {
        m_Serial->Close();
        delete m_Serial;
        m_Serial = NULL;
    }

    if (m_Queue != NULL)
    {
        m_Queue->Close();
        delete m_Queue;
        m_Queue = NULL;
    }

    return ResultEnum::NormalEnd;
}

ResultEnum MotorCommunicator::createSendData(const MotorCommandEnum command, const CutterDriveEnum cutter, char* const buffer)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    long tempCommand = 0;

    if (E_COMMAND_MAX <= command)
    {
        char log[40] = { 0 };
        snprintf(&log[0], sizeof(log), "[createSendData] Illegal command. [%d]\n", command);
        m_Logger->LOG_ERROR(log);
        goto FINISH;
    }

    if (E_CUTTER_TYPE_MAX <= cutter)
    {
        char log[40] = { 0 };
        snprintf(&log[0], sizeof(log), "[createSendData] Illegal cutter move. [%d]\n", cutter);
        m_Logger->LOG_ERROR(log);
        goto FINISH;
    }

    tempCommand = (long)cutter;
    tempCommand <<= 4;
    tempCommand |= ((long)command & 0x0000000F);

    buffer[0] = 0xFF;
    buffer[1] = 0x02;
    buffer[2] = (char)tempCommand;


FINISH :
    return retVal;
}

ResultEnum MotorCommunicator::analyze(char* const buffer)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    long tempCommand = 0;

    if (buffer[0] != 0xFF)
    {
        char log[40] = { 0 };
        snprintf(&log[0], sizeof(log), "[analyze] Illegal 1st Byte. [%02Xh]\n", buffer[0]);
        m_Logger->LOG_ERROR(log);
        goto FINISH;
    }

    if (buffer[1] != 0x01)
    {
        char log[40] = { 0 };
        snprintf(&log[0], sizeof(log), "[analyze] Illegal 2nd Byte. [%02Xh]\n", buffer[0]);
        m_Logger->LOG_ERROR(log);
        goto FINISH;
    }

    tempCommand = buffer[2] & 0x0F;
    pShareMemory->MotorState.Command = (MotorCommandEnum)tempCommand;
    tempCommand = (buffer[2] >> 4) & 0x0F;
    pShareMemory->MotorState.Cutter = (CutterDriveEnum)tempCommand;

    tempCommand = buffer[3];
    tempCommand <<= 8;
    tempCommand |= buffer[4];
    pShareMemory->MotorState.PointX = tempCommand;

    tempCommand = buffer[5];
    tempCommand <<= 8;
    tempCommand |= buffer[6];
    pShareMemory->MotorState.PointY = tempCommand;

    if ((buffer[7] & 0x01) != 0)
    {
        pShareMemory->MotorState.ErrorStatus = 1;
    }
    else
    {
        pShareMemory->MotorState.ErrorStatus = 0;
    }

    if ((buffer[7] & 0x02) != 0)
    {
        pShareMemory->MotorState.RemoteMode = E_MODE_MANUAL;
    }
    else
    {
        pShareMemory->MotorState.RemoteMode = E_MODE_AUTO;
    }


FINISH :
    return retVal;
}

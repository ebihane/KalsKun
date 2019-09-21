#include <stdio.h>
#include <stdlib.h>
#include "Parts/ShareMemory/ShareMemory.h"
#include "MotorCommunicator.h"

MotorCommunicator::MotorCommunicator()
    : LoopThreadBase(100, TypeEnum::TIMER_STOP)
    , m_Serial(NULL)
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

    /* シリアルポート設定 */
    serialSetting.Baudrate = Serial::BaudrateEnum::E_Baudrate_115200;
    serialSetting.Parity = Serial::ParityEnum::E_Parity_Non;
    serialSetting.StopBit = Serial::StopBitEnum::E_StopBit_1Bit;
    serialSetting.DataLength = Serial::DataLengthEnum::E_Data_8bit;

    m_Serial = new Serial((char*)"ttyUSB0", &serialSetting);
    if (m_Serial == NULL)
    {
        m_Logger->LOG_ERROR("[initializeCore] Serial allocation failed.\n");
        goto FINISH;
    }

    if (m_Serial->Open() != ResultEnum::NormalEnd)
    {
        m_Logger->LOG_ERROR("[initializeCore] Serial Open failed.\n");
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;

FINISH:
    return retVal;
}

ResultEnum MotorCommunicator::doMainProc()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    long tempCommand = 0;
    char sendBuffer[3] = { 0 };
    char recvBuffer[8] = { 0 };


    tempCommand = pShareMemory->MotorDrive.Cutter;
    tempCommand <<= 4;
    tempCommand |= (pShareMemory->MotorDrive.Command & 0x0000000F);

    sendBuffer[0] = 0xFF;
    sendBuffer[1] = 0x02;
    sendBuffer[2] = (char)tempCommand;

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

    tempCommand = recvBuffer[2] & 0x0F;
    pShareMemory->MotorState.Command = (MotorCommandEnum)tempCommand;
    tempCommand = (recvBuffer[2] >> 4) & 0x0F;
    pShareMemory->MotorState.Cutter = (CutterDriveEnum)tempCommand;

    tempCommand = recvBuffer[3];
    tempCommand <<= 8;
    tempCommand |= recvBuffer[4];
    pShareMemory->MotorState.PointX = tempCommand;

    tempCommand = recvBuffer[5];
    tempCommand <<= 8;
    tempCommand |= recvBuffer[6];
    pShareMemory->MotorState.PointY = tempCommand;

    if ((recvBuffer[7] & 0x01) != 0)
    {
        pShareMemory->MotorState.ErrorStatus = 1;
    }
    else
    {
        pShareMemory->MotorState.ErrorStatus = 0;
    }

    if ((recvBuffer[7] & 0x02) != 0)
    {
        pShareMemory->MotorState.RemoteMode = E_MODE_MANUAL;
    }
    else
    {
        pShareMemory->MotorState.RemoteMode = E_MODE_AUTO;
    }

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

    return ResultEnum::NormalEnd;
}

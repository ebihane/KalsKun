#include <stdlib.h>
#include "Parts/ShareMemory/ShareMemory.h"
#include "MotorCommunicator.h"

MotorCommunicator::MotorCommunicator(AdapterBase* const adapter)
 : ReceiverThread(adapter)
 , m_Serial(NULL)
 , m_SendTiming(false)
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
    serialSetting.Baudrate = Serial::BaudrateEnum::E_Baudrate_115200;
    serialSetting.Parity = Serial::ParityEnum::E_Parity_Non;
    serialSetting.StopBit = Serial::StopBitEnum::E_StopBit_1Bit;
    serialSetting.DataLength = Serial::DataLengthEnum::E_Data_8bit;

    Serial* serial = new Serial((char*)"ttyUSB0", &serialSetting);
    if (serial == NULL)
    {
        m_Logger->LOG_ERROR("[initializeCore] serial allocation failed.\n");
        goto FINISH;
    }

    m_Serial = serial;
    m_RecvData = new char[20];
    
    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

bool MotorCommunicator::isReceiveComplete(char* const buffer, const unsigned long size)
{
    return true;
}

ResultEnum MotorCommunicator::analyze(char* const buffer)
{
    char sendBuffer[20] = { 0 };

    memcpy(&sendBuffer[0], buffer, )


}

bool MotorCommunicator::createSendData(char* const data, unsigned long* size)
{
    bool retVal = false;

    if (m_SendTiming == false)
    {
        /* Ÿ‚Í‘—M‚·‚é */
        m_SendTiming = true;

        *size = 0;
        goto FINISH;
    }

    data[0] = 0xFF;
    data[1] = 0x02;
    data[2] = pShareMemory->MotorCommand.CutterSpeed;
    data[3] = pShareMemory->MotorCommand.FrontBack;
    data[4] = pShareMemory->MotorCommand.LeftRight;

    *size = 5;

    /* Ÿ‚Í‘—M‚µ‚È‚¢ */
    m_SendTiming = false;

    retVal = true;

FINISH :

    return retVal;
}

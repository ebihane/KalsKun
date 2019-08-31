#include "Parts/ShareMemory/ShareMemory.h"
#include "MotorCommunicator.h"

MotorCommunicator::MotorCommunicator(AdapterBase* const adapter)
 : SenderThread(adapter)
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
    m_SendData = new char[5];
    return ResultEnum::NormalEnd;
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

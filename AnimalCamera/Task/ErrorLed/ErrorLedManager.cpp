#include "Parts/AnimalCameraCommon.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "ErrorLedManager.h"

ErrorLedManager::ErrorLedManager()
 : LoopThreadBase((char*)"ErrorLed", 500, TypeEnum::CYCLIC)
 , m_CurrentLevel(LOW)
{
    /* nop. */
}

ErrorLedManager::~ErrorLedManager()
{
    /* nop. */
}

ResultEnum ErrorLedManager::doMainProc()
{
    if (pShareMemory->SystemError == false)
    {
        digitalWrite(IO_ERROR_LED, HIGH);
    }
    else if (m_CurrentLevel == LOW)
    {
        digitalWrite(IO_ERROR_LED, HIGH);
        m_CurrentLevel = HIGH;
    }
    else
    {
        digitalWrite(IO_ERROR_LED, LOW);
        m_CurrentLevel = LOW;
    }

    return ResultEnum::NormalEnd;
}
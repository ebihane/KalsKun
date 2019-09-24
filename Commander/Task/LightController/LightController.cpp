#include "Parts/CommanderCommon.h"
#include "LightController.h"

LightController::LightController()
 : LoopThreadBase((char*)"LightCont", 500, TypeEnum::TIMER_STOP)
 , m_OnOff(true)
{
    /* nop. */
}

LightController::~LightController()
{
    /* nop. */
}

ResultEnum LightController::initializeCore()
{
    pShareMemory->Commander.LightMode = LightModeEnum::E_LIGHT_OFF;
    return ResultEnum::NormalEnd;
}

ResultEnum LightController::doMainProc()
{
    LightModeEnum mode = pShareMemory->Commander.LightMode;

    if (mode == LightModeEnum::E_LIGHT_ON)
    {
        digitalWrite(IO_DIRECTION_LIGHT, HIGH);
        m_OnOff = true;
    }
    else if (mode == LightModeEnum::E_LIGHT_BLINK)
    {
        if (m_OnOff == true)
        {
            digitalWrite(IO_DIRECTION_LIGHT, LOW);
            m_OnOff = false;
        }
        else
        {
            digitalWrite(IO_DIRECTION_LIGHT, HIGH);
            m_OnOff = true;
        }
    }
    else
    {
        digitalWrite(IO_DIRECTION_LIGHT, LOW);
        m_OnOff = false;
    }

    return ResultEnum::NormalEnd;
}

ResultEnum LightController::finalizeCore()
{
    digitalWrite(IO_DIRECTION_LIGHT, LOW);
    pShareMemory->Commander.LightMode = LightModeEnum::E_LIGHT_OFF;
    return ResultEnum::NormalEnd;
}

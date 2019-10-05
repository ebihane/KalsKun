#include "Parts/AnimalCameraCommon.h"
#include "HeartBeatManager.h"

HeartBeatManager::HeartBeatManager()
 : LoopThreadBase((char*)"HeartBeat", 300, TypeEnum::CYCLIC)
 , m_CurrentLevel(LOW)
{
    /* nop. */
}

HeartBeatManager::~HeartBeatManager()
{
    /* nop. */
}

ResultEnum HeartBeatManager::doMainProc()
{
    if (m_CurrentLevel == HIGH)
    {
        m_CurrentLevel = LOW;
    }
    else
    {
        m_CurrentLevel = HIGH;
    }

    digitalWrite(IO_HEART_BEAT, m_CurrentLevel);

    return ResultEnum::NormalEnd;
}

ResultEnum HeartBeatManager::finalizeCore()
{
    digitalWrite(IO_HEART_BEAT, LOW);
    return ResultEnum::NormalEnd;
}

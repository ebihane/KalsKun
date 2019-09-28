#include "Parts/AroundCameraCommon.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "RedwavePatrol.h"

RedwavePatrol::RedwavePatrol()
 : LoopThreadBase((char*)"RedWavePatrol", 300, TypeEnum::TIMER_STOP)
 , m_BeforeDetected(false)
{
    /* nop. */
}

RedwavePatrol::~RedwavePatrol()
{
    /* nop. */
}

ResultEnum RedwavePatrol::initializeCore()
{
    pShareMemory->Detect = DetectTypeEnum::NOT_DETECT;
    return ResultEnum::NormalEnd;
}

ResultEnum RedwavePatrol::doMainProc()
{
    bool detected = false;

    int state1 = digitalRead(IO_REDWAVE_MON_1);
    int state2 = digitalRead(IO_REDWAVE_MON_2);
    int state3 = digitalRead(IO_REDWAVE_MON_3);
    int state4 = digitalRead(IO_REDWAVE_MON_4);

    if ((state1 != LOW)
    ||  (state2 != LOW)
    ||  (state3 != LOW)
    ||  (state4 != LOW))
    {
        detected = true;
    }
    else
    {
        detected = false;
    }

    if (detected == true)
    {
        pShareMemory->Detect = DetectTypeEnum::DETECTED;
    }
    else
    {
        if (m_BeforeDetected == true)
        {
            m_NormalReturnWatch.Start();
        }
        
        if (5.0f <= m_NormalReturnWatch.GetSplit())
        {
            pShareMemory->Detect = DetectTypeEnum::NOT_DETECT;
        }
    }

    m_BeforeDetected = detected;

    return ResultEnum::NormalEnd;
}

ResultEnum RedwavePatrol::finalizeCore()
{
    pShareMemory->Detect = DetectTypeEnum::NOT_DETECT;
    return ResultEnum::NormalEnd;
}

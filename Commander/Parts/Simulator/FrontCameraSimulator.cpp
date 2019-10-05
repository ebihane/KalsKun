#include <string.h>
#include "Parts/ShareMemory/ShareMemory.h"
#include "Parts/CommanderCommon.h"
#include "FrontCameraSimulator.h"

FrontCameraSimulator::FrontCameraSimulator()
 : AdapterBase()
 , m_ReceiveCount(0)
 , m_AvoidPrevLevel(LOW)
 , m_TapePrevLevel(LOW)
{
    /* nop. */
}

FrontCameraSimulator::~FrontCameraSimulator()
{
    /* nop. */
}

ResultEnum FrontCameraSimulator::Open()
{
    return ResultEnum::NormalEnd;
}

ResultEnum FrontCameraSimulator::Close()
{
    return ResultEnum::NormalEnd;
}

ResultEnum FrontCameraSimulator::Connection()
{
    return ResultEnum::NormalEnd;
}

ResultEnum FrontCameraSimulator::Disconnection()
{
    return ResultEnum::NormalEnd;
}

ResultEnum FrontCameraSimulator::IsSendable(bool& sendable)
{
    sendable = true;
    return ResultEnum::NormalEnd;
}

ResultEnum FrontCameraSimulator::Send(void* const bufferPtr, const unsigned long size)
{
    return ResultEnum::NormalEnd;
}

ResultEnum FrontCameraSimulator::IsReceivable(bool& receivable)
{
    receivable = true;
    return ResultEnum::NormalEnd;
}

ResultEnum FrontCameraSimulator::Receive(void* const bufferPtr, const unsigned long size)
{
    delay(10);
    
    MoveTypeEnum type = MoveTypeEnum::NOT_REQUEST;
    DetectTypeEnum tape = DetectTypeEnum::NOT_DETECT;
    DetectTypeEnum avoid = DetectTypeEnum::NOT_DETECT;
    float distance = 500.0f;

    int level = digitalRead(IO_SIMULATOR_AVOID);
    if ((level == HIGH) && (m_AvoidPrevLevel == LOW))
    {
        avoid = DetectTypeEnum::DETECTED;
        distance = 30.0f;
        type = MoveTypeEnum::AVOIDANCE;
    }

    m_AvoidPrevLevel = level;

    level = digitalRead(IO_SIMULATOR_TAPE);
    if ((level == HIGH) && (m_TapePrevLevel == LOW))
    {
        tape = DetectTypeEnum::DETECTED;
        type = MoveTypeEnum::TURN;
    }

    m_TapePrevLevel = level;

    EventInfo ev = { 0 };
    ev.lParam[0] = m_ReceiveCount;
    ev.lParam[1] = 0;
    ev.lParam[2] = (long)type;
    ev.lParam[3] = (long)tape;
    ev.lParam[4] = (long)avoid;
    ev.fParam[0] = distance;
    ev.fParam[1] = 500.0f;

    memcpy(bufferPtr, &ev, sizeof(EventInfo));

    return ResultEnum::NormalEnd;
}

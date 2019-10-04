#include <string.h>
#include "Parts/ShareMemory/ShareMemory.h"
#include "FrontCameraSimulator.h"

FrontCameraSimulator::FrontCameraSimulator()
 : AdapterBase()
 , m_ReceiveCount(0)
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

    EventInfo ev = { 0 };
    ev.lParam[0] = m_ReceiveCount;
    ev.lParam[1] = 0;
    ev.lParam[2] = (long)MoveTypeEnum::NOT_REQUEST;
    ev.lParam[3] = (long)DetectTypeEnum::NOT_DETECT;
    ev.lParam[4] = (long)DetectTypeEnum::NOT_DETECT;
    ev.fParam[0] = 500.0f;
    ev.fParam[1] = 500.0f;

    memcpy(bufferPtr, &ev, sizeof(EventInfo));

    return ResultEnum::NormalEnd;
}

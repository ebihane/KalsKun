#include <string.h>
#include "Parts/ShareMemory/ShareMemory.h"
#include "AroundCameraSimulator.h"

AroundCameraSimulator::AroundCameraSimulator()
 : AdapterBase()
 , m_ReceiveCount(0)
{
    /* nop. */
}

AroundCameraSimulator::~AroundCameraSimulator()
{
    /* nop. */
}

ResultEnum AroundCameraSimulator::Open()
{
    return ResultEnum::NormalEnd;
}

ResultEnum AroundCameraSimulator::Close()
{
    return ResultEnum::NormalEnd;
}

ResultEnum AroundCameraSimulator::Connection()
{
    return ResultEnum::NormalEnd;
}

ResultEnum AroundCameraSimulator::Disconnection()
{
    return ResultEnum::NormalEnd;
}

ResultEnum AroundCameraSimulator::IsSendable(bool& sendable)
{
    sendable = true;
    return ResultEnum::NormalEnd;
}

ResultEnum AroundCameraSimulator::Send(void* const bufferPtr, const unsigned long size)
{
    return ResultEnum::NormalEnd;
}

ResultEnum AroundCameraSimulator::IsReceivable(bool& receivable)
{
    receivable = true;
    return ResultEnum::NormalEnd;
}

ResultEnum AroundCameraSimulator::Receive(void* const bufferPtr, const unsigned long size)
{
    delay(10);

    EventInfo ev = { 0 };
    ev.lParam[0] = m_ReceiveCount;
    ev.lParam[1] = 0;
    ev.lParam[2] = (long)DetectTypeEnum::NOT_DETECT;

    memcpy(bufferPtr, &ev, sizeof(EventInfo));

    return ResultEnum::NormalEnd;
}

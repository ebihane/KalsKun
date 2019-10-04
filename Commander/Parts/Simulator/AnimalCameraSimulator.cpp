#include <string.h>
#include "Parts/ShareMemory/ShareMemory.h"
#include "AnimalCameraSimulator.h"

AnimalCameraSimulator::AnimalCameraSimulator()
 : AdapterBase()
 , m_ReceiveCount(0)
{
    /* nop. */
}

AnimalCameraSimulator::~AnimalCameraSimulator()
{
    /* nop. */
}

ResultEnum AnimalCameraSimulator::Open()
{
    return ResultEnum::NormalEnd;
}

ResultEnum AnimalCameraSimulator::Close()
{
    return ResultEnum::NormalEnd;
}

ResultEnum AnimalCameraSimulator::Connection()
{
    return ResultEnum::NormalEnd;
}

ResultEnum AnimalCameraSimulator::Disconnection()
{
    return ResultEnum::NormalEnd;
}

ResultEnum AnimalCameraSimulator::IsSendable(bool& sendable)
{
    sendable = true;
    return ResultEnum::NormalEnd;
}

ResultEnum AnimalCameraSimulator::Send(void* const bufferPtr, const unsigned long size)
{
    return ResultEnum::NormalEnd;
}

ResultEnum AnimalCameraSimulator::IsReceivable(bool& receivable)
{
    receivable = true;
    return ResultEnum::NormalEnd;
}

ResultEnum AnimalCameraSimulator::Receive(void* const bufferPtr, const unsigned long size)
{
    delay(10);

    EventInfo ev = { 0 };
    ev.lParam[0] = m_ReceiveCount;
    ev.lParam[1] = 0;
    ev.lParam[2] = (long)DetectTypeEnum::NOT_DETECT;
    ev.lParam[3] = (long)DetectTypeEnum::NOT_DETECT;

    memcpy(bufferPtr, &ev, sizeof(EventInfo));

    return ResultEnum::NormalEnd;
}

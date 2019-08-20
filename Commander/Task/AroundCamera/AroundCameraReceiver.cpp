#include "Parts/ShareMemory/ShareMemory.h"
#include "AroundCameraReceiver.h"

AroundCameraReceiver::AroundCameraReceiver()
 : ReceiverThread(AC_TO_COMMANDER_PORT)
{
    /* nop. */
}

AroundCameraReceiver::~AroundCameraReceiver()
{
    /* nop. */
}

ResultEnum AroundCameraReceiver::analyze(EventInfo* const ev)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    pShareMemory->AroundCamera.ReceiveCount = ev->lParam[0];
    pShareMemory->AroundCamera.SystemError = ev->lParam[1];
    pShareMemory->AroundCamera.PersonDetect = ev->lParam[2];

    retVal = ResultEnum::NormalEnd;

    return retVal;
}

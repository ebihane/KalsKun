#include "Parts/ShareMemory/ShareMemory.h"
#include "FrontCameraReceiver.h"

FrontCameraReceiver::FrontCameraReceiver(AdapterBase* const adapter)
 : ReceiverThread((char*)"FC_Receiver", adapter)
{
    /* nop. */
}

FrontCameraReceiver::~FrontCameraReceiver()
{
    /* nop. */
}

ResultEnum FrontCameraReceiver::initializeCore()
{
    EventInfo* p = new EventInfo;
    m_RecvData = (char*)p;

    return ResultEnum::NormalEnd;
}

bool FrontCameraReceiver::isReceiveComplete(char* const buffer, const unsigned long size)
{
    bool retVal = false;

    if (sizeof(EventInfo) <= size)
    {
        retVal = true;
    }

    return retVal;
}

ResultEnum FrontCameraReceiver::analyze(char* const buffer)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    EventInfo* p = (EventInfo*)buffer;

    pShareMemory->FrontCamera.ReceiveCount = p->lParam[0];
    pShareMemory->FrontCamera.SystemError = p->lParam[1];
    pShareMemory->FrontCamera.Avoidance = p->lParam[2];

    retVal = ResultEnum::NormalEnd;

    return retVal;
}

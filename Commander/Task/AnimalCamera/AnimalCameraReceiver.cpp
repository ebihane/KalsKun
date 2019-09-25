#include "Parts/ShareMemory/ShareMemory.h"
#include "AnimalCameraReceiver.h"

AnimalCameraReceiver::AnimalCameraReceiver(AdapterBase* const adapter)
 : ReceiverThread((char*)"AnimalReceiver", adapter)
{
    /* nop. */
}

AnimalCameraReceiver::~AnimalCameraReceiver()
{
    /* nop. */
}

ResultEnum AnimalCameraReceiver::initializeCore()
{
    EventInfo* p = new EventInfo;
    m_RecvData = (char*)p;

    return ResultEnum::NormalEnd;
}

bool AnimalCameraReceiver::isReceiveComplete(char* const buffer, const unsigned long size)
{
    bool retVal = false;

    if (sizeof(EventInfo) <= size)
    {
        retVal = true;
    }

    return retVal;
}

ResultEnum AnimalCameraReceiver::analyze(char* const buffer)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    EventInfo* p = (EventInfo*)buffer;

    pShareMemory->AnimalCamera.ReceiveCount = p->lParam[0];
    pShareMemory->AnimalCamera.SystemError = p->lParam[1];
    pShareMemory->AnimalCamera.Detection = p->lParam[2];

    retVal = ResultEnum::NormalEnd;

    return retVal;
}

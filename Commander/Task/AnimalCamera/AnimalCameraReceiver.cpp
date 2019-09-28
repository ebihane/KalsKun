#include <string.h>
#include "Parts/CommanderCommon.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "AnimalCameraReceiver.h"

AnimalCameraReceiver::AnimalCameraReceiver(AdapterBase* const adapter)
 : ReceiverThread((char*)"AnimalReceiver", adapter, true)
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

    EventInfo* q = new EventInfo;
    m_ResponseData = (char*)q;

    return ResultEnum::NormalEnd;
}

void AnimalCameraReceiver::doReconnectInitialize(const bool isFirst)
{
    /* 再接続時はシステムエラー扱いとする */
    if (isFirst != true)
    {
        pShareMemory->AnimalCamera.SystemError = 1;
    }
}

ResultEnum AnimalCameraReceiver::doConnectedProc()
{
    /* 接続完了により一時的にシステムエラー解除 */
    pShareMemory->AnimalCamera.SystemError = 0;
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
    pShareMemory->AnimalCamera.Human = (DetectTypeEnum)p->lParam[2];
    pShareMemory->AnimalCamera.Animal = (DetectTypeEnum)p->lParam[3];

    retVal = ResultEnum::NormalEnd;

    return retVal;
}

unsigned long AnimalCameraReceiver::createResponse(char* const buffer)
{
    EventInfo* p = (EventInfo*)buffer;

    memset(p, 0x00, sizeof(EventInfo));
    p->Result = ResultEnum::NormalEnd;

    return sizeof(EventInfo);
}

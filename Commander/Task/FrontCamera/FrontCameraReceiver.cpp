#include <string.h>
#include "Parts/CommanderCommon.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "FrontCameraReceiver.h"

FrontCameraReceiver::FrontCameraReceiver(AdapterBase* const adapter)
 : ReceiverThread((char*)"FC_Receiver", adapter, true)
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

    EventInfo* q = new EventInfo;
    m_ResponseData = (char*)q;

    return ResultEnum::NormalEnd;
}

void FrontCameraReceiver::doReconnectInitialize(const bool isFirst)
{
    /* 再接続時はシステムエラー扱いとする */
    if (isFirst != true)
    {
        pShareMemory->FrontCamera.SystemError = 1;
    }
}

ResultEnum FrontCameraReceiver::doConnectedProc()
{
    /* 接続完了により一時的にシステムエラー解除 */
    pShareMemory->FrontCamera.SystemError = 0;
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
    pShareMemory->FrontCamera.MoveType = (MoveTypeEnum)p->lParam[2];
    pShareMemory->FrontCamera.RedTape = (DetectTypeEnum)p->lParam[3];
    pShareMemory->FrontCamera.BlueObject = (DetectTypeEnum)p->lParam[4];
    pShareMemory->FrontCamera.Distance[0] = p->fParam[0];
    pShareMemory->FrontCamera.Distance[1] = p->fParam[1];

    retVal = ResultEnum::NormalEnd;

    return retVal;
}

unsigned long FrontCameraReceiver::createResponse(char* const buffer)
{
    EventInfo* p = (EventInfo*)buffer;

    memset(p, 0x00, sizeof(EventInfo));
    p->Result = ResultEnum::NormalEnd;

    return sizeof(EventInfo);
}

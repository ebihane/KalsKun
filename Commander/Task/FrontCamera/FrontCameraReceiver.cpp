#include <stdio.h>
#include <string.h>
#include "Parts/CommanderCommon.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "FrontCameraReceiver.h"

FrontCameraReceiver::FrontCameraReceiver(AdapterBase* const adapter)
 : ReceiverThread((char*)"FC_Receiver", adapter, true)
 , m_PrevMoveType(MoveTypeEnum::NOT_REQUEST)
 , m_PrevRedTape(DetectTypeEnum::NOT_DETECT)
 , m_PrevBlueObject(DetectTypeEnum::NOT_DETECT)
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

    if (pShareMemory->FrontCamera.MoveType != m_PrevMoveType)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[FC_Receiver] Move type change. [%d -> %d]\n", m_PrevMoveType, pShareMemory->FrontCamera.MoveType);
        m_Logger->LOG_INFO(m_LogStr);
    }

    if (pShareMemory->FrontCamera.RedTape != m_PrevRedTape)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[FC_Receiver] Red Tape change. [%d -> %d]\n", m_PrevRedTape, pShareMemory->FrontCamera.RedTape);
        m_Logger->LOG_INFO(m_LogStr);
    }

    if (pShareMemory->FrontCamera.BlueObject != m_PrevBlueObject)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[FC_Receiver] Blue Ojcect change. [%d -> %d]\n", m_PrevBlueObject, pShareMemory->FrontCamera.BlueObject);
        m_Logger->LOG_INFO(m_LogStr);
    }

    m_PrevMoveType = pShareMemory->FrontCamera.MoveType;
    m_PrevRedTape = pShareMemory->FrontCamera.RedTape;
    m_PrevBlueObject = pShareMemory->FrontCamera.BlueObject;

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

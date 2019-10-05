#include <stdio.h>
#include <string.h>
#include "Parts/CommanderCommon.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "AroundCameraReceiver.h"

AroundCameraReceiver::AroundCameraReceiver(AdapterBase* const adapter)
 : ReceiverThread((char*)"AC_Receiver", adapter, true)
 , m_PrevDetect(DetectTypeEnum::NOT_DETECT)
{
    /* nop. */
}

AroundCameraReceiver::~AroundCameraReceiver()
{
    /* nop. */
}

ResultEnum AroundCameraReceiver::initializeCore()
{
    EventInfo* p = new EventInfo;
    m_RecvData = (char*)p;

    EventInfo* q = new EventInfo;
    m_ResponseData = (char*)q;

    return ResultEnum::NormalEnd;
}

bool AroundCameraReceiver::isReceiveComplete(char* const buffer, const unsigned long size)
{
    bool retVal = false;

    if (sizeof(EventInfo) <= size)
    {
        retVal = true;
    }

    return retVal;
}

ResultEnum AroundCameraReceiver::analyze(char* const buffer)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    EventInfo* p = (EventInfo*)buffer;

    pShareMemory->AroundCamera.ReceiveCount = p->lParam[0];
    pShareMemory->AroundCamera.SystemError = p->lParam[1];
    pShareMemory->AroundCamera.Detect = (DetectTypeEnum)p->lParam[2];

    if (pShareMemory->AroundCamera.Detect != m_PrevDetect)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[AC_Receiver] Blue Ojcect change. [%d -> %d]\n", m_PrevDetect, pShareMemory->AroundCamera.Detect);
        m_Logger->LOG_INFO(m_LogStr);
    }

    m_PrevDetect = pShareMemory->AroundCamera.Detect;

    retVal = ResultEnum::NormalEnd;

    return retVal;
}

unsigned long AroundCameraReceiver::createResponse(char* const buffer)
{
    EventInfo* p = (EventInfo*)buffer;

    memset(p, 0x00, sizeof(EventInfo));
    p->Result = ResultEnum::NormalEnd;

    return sizeof(EventInfo);
}

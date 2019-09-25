#include "Parts/ShareMemory/ShareMemory.h"
#include "StateSender.h"

StateSender::StateSender(AdapterBase* const adapter)
 : SenderThread((char*)"StateSender", adapter)
 , m_SendCount(0)
{
    /* nop. */
}

StateSender::~StateSender()
{
    /* nop. */
}

ResultEnum StateSender::initializeCore()
{
    EventInfo* p = new EventInfo();
    m_SendData = (char *)p;
    return ResultEnum::NormalEnd;
}

bool StateSender::createSendData(char* const data, unsigned long* const size)
{
    bool retVal = false;
    EventInfo* p = (EventInfo*)data;

    if (isStopRequest() == true)
    {
        goto FINISH;
    }

    m_SendCount++;

    p->Code = 1;
    p->Result = ResultEnum::NormalEnd;
    p->lParam[0] = m_SendCount;
    p->lParam[1] = pShareMemory->SystemError;
	p->lParam[2] = pShareMemory->StateData;
    p->fParam[0] = pShareMemory->UltrasoundData[0];
    p->fParam[1] = pShareMemory->UltrasoundData[1];

    *size = sizeof(EventInfo);

    retVal = true;

FINISH :
    return retVal;
}

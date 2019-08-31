#include "Parts/ShareMemory/ShareMemory.h"
#include "StateSender.h"

StateSender::StateSender(AdapterBase* const adapter)
 : SenderThread(adapter)
 , m_SendCount(0)
{
    /* nop. */
}

StateSender::~StateSender()
{
    /* nop. */
}

bool StateSender::createSendData(EventInfo* const ev)
{
    bool retVal = false;

    if (isStopRequest() == true)
    {
        goto FINISH;
    }

    m_SendCount++;

    ev->Code = 1;
    ev->Result = ResultEnum::NormalEnd;
    ev->lParam[0] = m_SendCount;
    ev->lParam[1] = pShareMemory->SystemError;
    ev->lParam[2] = (long)pShareMemory->PatrolState;

    retVal = true;

FINISH :
    return retVal;
}

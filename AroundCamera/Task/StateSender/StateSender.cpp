#include "Parts/ShareMemory/ShareMemory.h"
#include "StateSender.h"

StateSender::StateSender(AdapterBase* const adapter)
 : SenderThread((char*)"StateSender", adapter)
 , m_SendTiming(false)
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
    /* 司令塔マイコンより早く起動すると接続できない？？？ */
    /* とりあえず待ち処理 (5秒) を入れる */
    delay(5000);

    EventInfo* p = new EventInfo();
    m_SendData = (char*)p;
    return ResultEnum::NormalEnd;
}

bool StateSender::createSendData(char* const data, unsigned long* const size)
{
    bool retVal = false;
    EventInfo* p = (EventInfo*)data;

    if (m_SendTiming == false)
    {
        m_SendTiming = true;
        goto FINISH;
    }

    m_SendCount++;

    p->Code = 1;
    p->Result = ResultEnum::NormalEnd;
    p->lParam[0] = m_SendCount;
    p->lParam[1] = pShareMemory->SystemError;
    p->lParam[2] = (long)pShareMemory->PatrolState;

    *size = sizeof(EventInfo);

    m_SendTiming = false;
    retVal = true;

FINISH :
    return retVal;
}

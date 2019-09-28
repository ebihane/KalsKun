#include "Parts/ShareMemory/ShareMemory.h"
#include "StateSender.h"

StateSender::StateSender(AdapterBase* const adapter)
 : SenderThread((char*)"StateSender", adapter, true)
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
    m_SendData = (char *)p;

    EventInfo* q = new EventInfo();
    m_RecvData = (char *)q;
    m_RecvSize = sizeof(EventInfo);
    
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
	p->lParam[2] = pShareMemory->MoveType;
    p->lParam[3] = pShareMemory->RedTape;
    p->lParam[4] = pShareMemory->BlueObject;
    p->fParam[0] = pShareMemory->UltrasoundData[0];
    p->fParam[1] = pShareMemory->UltrasoundData[1];

    *size = sizeof(EventInfo);

    retVal = true;

FINISH :
    return retVal;
}

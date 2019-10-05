#include <stdio.h>
#include <string.h>
#include "Parts/CommanderCommon.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "AnimalCameraReceiver.h"

AnimalCameraReceiver::AnimalCameraReceiver(AdapterBase* const adapter)
 : ReceiverThread((char*)"AnimalReceiver", adapter, true)
 , m_PrevHuman(DetectTypeEnum::NOT_DETECT)
 , m_PrevAnimal(DetectTypeEnum::NOT_DETECT)
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

    if (pShareMemory->AnimalCamera.Human != m_PrevHuman)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[Animal_Receiver] Human Detect change. [%d -> %d]\n", m_PrevHuman, pShareMemory->AnimalCamera.Human);
        m_Logger->LOG_INFO(m_LogStr);
    }

    if (pShareMemory->AnimalCamera.Animal != m_PrevAnimal)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[Animal_Receiver] Animal Detect change. [%d -> %d]\n", m_PrevAnimal, pShareMemory->AnimalCamera.Animal);
        m_Logger->LOG_INFO(m_LogStr);
    }

    m_PrevHuman = pShareMemory->AnimalCamera.Human;
    m_PrevAnimal = pShareMemory->AnimalCamera.Animal;

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

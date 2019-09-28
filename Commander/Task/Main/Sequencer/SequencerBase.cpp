#include <stdio.h>
#include "Parts/CommanderCommon.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "SequencerBase.h"

SequencerBase::SequencerBase(const SequenceTypeEnum type)
 : MY_SEQUENCE_TYPE(type)
 , m_Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::BOTH_OUT)
{
    /* nop. */
}

SequencerBase::~SequencerBase()
{
    /* nop. */
}

void SequencerBase::Initialize(const SequenceTypeEnum before)
{
    m_BeforeSequence = before;
    initializeCore();
}

void SequencerBase::Destroy()
{
    destroyCore();
}

SequencerBase::SequenceTypeEnum SequencerBase::Process()
{
    SequenceTypeEnum retVal = MY_SEQUENCE_TYPE;


    retVal = processCore();

    /* 別マイコンでシステム異常発生 */
    if ((pShareMemory->FrontCamera.SystemError != 0)
    ||  (pShareMemory->AnimalCamera.SystemError != 0)
    ||  (pShareMemory->AroundCamera.SystemError != 0))
    {
        if (GetSequence() != SequenceTypeEnum::E_SEQ_ERROR)
        {
            char logStr[LOG_OUT_MAX] = { 0 };
            snprintf(&logStr[0], sizeof(logStr), "[Sequence] System Error detect!! [%ld/%ld/%ld]\n", pShareMemory->FrontCamera.SystemError, pShareMemory->AnimalCamera.SystemError, pShareMemory->AroundCamera.SystemError);
            m_Logger.LOG_ERROR(logStr);
        }

        retVal = SequenceTypeEnum::E_SEQ_ERROR;
        goto FINISH;
    }

    /* 司令塔マイコンで異常発生 */
    if (pShareMemory->Commander.SystemError != false)
    {
        if (GetSequence() != SequenceTypeEnum::E_SEQ_ERROR)
        {
            char logStr[LOG_OUT_MAX] = { 0 };
            snprintf(&logStr[0], sizeof(logStr), "[Sequence] Commander System Error detect!!\n");
            m_Logger.LOG_ERROR(logStr);
        }

        retVal = SequenceTypeEnum::E_SEQ_ERROR;
        goto FINISH;
    }

FINISH:
    return retVal;
}

SequencerBase::SequenceTypeEnum SequencerBase::GetSequence()
{
    return MY_SEQUENCE_TYPE;
}

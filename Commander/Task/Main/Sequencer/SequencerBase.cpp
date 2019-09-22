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


    /* 別マイコンでシステム異常発生 */
    if ((pShareMemory->FrontCamera.SystemError != 0)
    || (pShareMemory->AroundCamera.SystemError != 0))
    {
        retVal = SequenceTypeEnum::E_SEQ_ERROR;
        goto FINISH;
    }

    retVal = processCore();

FINISH:
    return retVal;
}

SequencerBase::SequenceTypeEnum SequencerBase::GetSequence()
{
    return MY_SEQUENCE_TYPE;
}

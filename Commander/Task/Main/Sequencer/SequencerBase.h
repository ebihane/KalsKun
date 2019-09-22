#pragma once

#include "Include/Common.h"
#include "Queue/SendQueue.h"
#include "Logger/Logger.h"
#include "Parts/ShareMemory/ShareMemory.h"

class SequencerBase
{
public :

    typedef enum
    {
        E_SEQ_IDLE = 0,
        E_SEQ_KUSAKARI,
        E_SEQ_YAKEI,
        E_SEQ_ERROR,
    } SequenceTypeEnum;

    SequencerBase(const SequenceTypeEnum type);
    virtual ~SequencerBase();

    void Initialize(const SequenceTypeEnum before);
    void Destroy();
    SequenceTypeEnum Process();
    SequenceTypeEnum GetSequence();

protected :

    const SequenceTypeEnum MY_SEQUENCE_TYPE;

    Logger m_Logger;
    SendQueue m_SendQueue;
    SequenceTypeEnum m_BeforeSequence;

    virtual ResultEnum initializeCore() = 0;
    virtual void destroyCore() = 0;
    virtual SequenceTypeEnum processCore() = 0;

private :

};

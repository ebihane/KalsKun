#pragma once

#include "Task/Main/Sequencer/SequencerBase.h"

class HimajinKun : public SequencerBase
{
public :

    HimajinKun();
    virtual ~HimajinKun();

protected :


private :

    ControlModeEnum m_LastRemoteType;

    ResultEnum initializeCore();
    void destroyCore();
    SequenceTypeEnum processCore();
};

#pragma once

#include "Task/Main/Sequencer/SequencerBase.h"

class YakeiKun : public SequencerBase
{
public:

    YakeiKun();
    virtual ~YakeiKun();

protected:


private:

    ResultEnum initializeCore();
    void destroyCore();
    SequenceTypeEnum processCore();

};

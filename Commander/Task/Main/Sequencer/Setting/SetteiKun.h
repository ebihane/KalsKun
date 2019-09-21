#pragma once

#include "Task/Main/Sequencer/SequencerBase.h"

class SetteiKun : public SequencerBase
{
public:

    SetteiKun();
    virtual ~SetteiKun();

protected:


private:

    void initializeCore();
    void destroyCore();
    SequenceTypeEnum processCore();

};

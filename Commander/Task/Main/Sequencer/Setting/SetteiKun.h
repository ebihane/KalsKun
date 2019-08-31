#pragma once

#include "Task/Main/Sequencer/SequencerBase.h"

class SetteiKun : public SequencerBase
{
public:

    SetteiKun();
    virtual ~SetteiKun();

    void Initialize();
    void Destroy();
    SequenceTypeEnum Process();

protected:


private:


};

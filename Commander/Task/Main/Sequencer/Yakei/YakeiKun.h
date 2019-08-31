#pragma once

#include "Task/Main/Sequencer/SequencerBase.h"

class YakeiKun : public SequencerBase
{
public:

    YakeiKun();
    virtual ~YakeiKun();

    void Initialize();
    void Destroy();
    SequenceTypeEnum Process();

protected:


private:


};

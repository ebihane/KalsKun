#pragma once

#include "Task/Main/Sequencer/SequencerBase.h"

class HimajinKun : public SequencerBase
{
public :

    HimajinKun();
    virtual ~HimajinKun();

    void Initialize();
    void Destroy();
    SequenceTypeEnum Process();

protected :


private :


};

#pragma once

#include "Task/Main/Sequencer/SequencerBase.h"

class KusakariKun : public SequencerBase
{
public:

    KusakariKun();
    virtual ~KusakariKun();

    void Initialize();
    void Destroy();
    SequenceTypeEnum Process();

protected:


private:


};

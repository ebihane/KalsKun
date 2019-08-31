#pragma once

#include "Task/Main/Sequencer/SequencerBase.h"

class ErrorKun : public SequencerBase
{
public:

    ErrorKun();
    virtual ~ErrorKun();

    void Initialize();
    void Destroy();
    SequenceTypeEnum Process();

protected:


private:


};

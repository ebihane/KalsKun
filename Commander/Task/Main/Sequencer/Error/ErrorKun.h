#pragma once

#include "Task/Main/Sequencer/SequencerBase.h"

class ErrorKun : public SequencerBase
{
public:

    ErrorKun();
    virtual ~ErrorKun();

protected:


private:

    ResultEnum initializeCore();
    void destroyCore();
    SequenceTypeEnum processCore();

};

#pragma once

#include "Task/Main/Sequencer/SequencerBase.h"

class KusakariKun : public SequencerBase
{
public:

    KusakariKun();
    virtual ~KusakariKun();

protected:


private:

    long m_TapeDetect;
    long m_PrevAvoidance;
    RectStr m_PrevRect;

    ResultEnum initializeCore();
    void destroyCore();
    SequenceTypeEnum processCore();

};

#pragma once

#include "Task/BuzzerController/BuzzerController.h"
#include "Task/LightController/LightController.h"
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
    BuzzerController* m_BuzzerThread;
    LightController* m_LightThread;

    ResultEnum initializeCore();
    void destroyCore();
    SequenceTypeEnum processCore();

    RectStr convertRealPointToMapPoint(SizeStr* const pRealPoint);
};

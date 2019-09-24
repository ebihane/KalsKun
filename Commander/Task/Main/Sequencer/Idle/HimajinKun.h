#pragma once

#include "Task/Main/Sequencer/SequencerBase.h"

#include "Task/BuzzerController/BuzzerController.h"
#ifdef IO_TEST
#include "Task/LightController/LightController.h"
#endif

class HimajinKun : public SequencerBase
{
public :

    HimajinKun();
    virtual ~HimajinKun();

protected :


private :

    ControlModeEnum m_LastRemoteType;

    BuzzerController* m_Buzzer;
#ifdef IO_TEST
    LightController* m_Light;
#endif

    ResultEnum initializeCore();
    void destroyCore();
    SequenceTypeEnum processCore();
};

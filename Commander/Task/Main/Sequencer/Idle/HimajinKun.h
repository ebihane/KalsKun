#pragma once

#include "Task/Main/Sequencer/SequencerBase.h"
#include "Parts/Setting/SettingManager.h"

class HimajinKun : public SequencerBase
{
public :

    HimajinKun();
    virtual ~HimajinKun();

protected :


private :

    SettingManager* m_SettingManager;

    ResultEnum initializeCore();
    void destroyCore();
    SequenceTypeEnum processCore();

    bool isTimeArrival(SettingManager::TimeSettingStr* current, SettingManager::TimeSettingStr* target);

};

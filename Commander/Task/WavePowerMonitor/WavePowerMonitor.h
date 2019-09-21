#pragma once

#include "ThreadBase/LoopThreadBase.h"

class WavePowerMonitor : public LoopThreadBase
{
public :

    WavePowerMonitor(const unsigned char apNo);
    virtual ~WavePowerMonitor();

protected :


private :

    /* ïΩãœâªâÒêîÅF10 âÒ */
    static const unsigned char  AVERAGE_COUNT = 10;

    const unsigned char         AP_INDEX;

    unsigned char   m_Index;
    long            m_RssiArray[AVERAGE_COUNT];
    long            m_TxPowerArray[AVERAGE_COUNT];
    
    ResultEnum initializeCore();
    ResultEnum doMainProc();
    float calcurateDistance();

};

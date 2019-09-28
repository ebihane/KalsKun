#pragma once

#include "ThreadBase/LoopThreadBase.h"
#include "Parts/ShareMemory/ShareMemory.h"

class BuzzerController : public LoopThreadBase
{
public :

    BuzzerController();
    virtual ~BuzzerController();

protected :


private :

    /* �������\���� */
    typedef struct
    {
        float Frequency;    /* ���g�� */
        float Time;         /* �L�[�v���鎞�� (sec) */
    } MelodyLineStr;

    static const MelodyLineStr  MELODY_1_TABLE[44];
    static const MelodyLineStr  MELODY_2_TABLE[120];

    Stopwatch       m_Watch;
    float           m_ThreshTime;
    MelodyModeEnum  m_Mode;
    unsigned long   m_Index;
    unsigned long   m_IndexMax;

    ResultEnum initializeCore();
    ResultEnum doMainProc();
    ResultEnum finalizeCore();

};

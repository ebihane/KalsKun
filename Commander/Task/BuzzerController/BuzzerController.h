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

    Stopwatch       m_Watch;
    MelodyModeEnum  m_Mode;
    long            m_Index;

    ResultEnum initializeCore();
    ResultEnum doMainProc();
    ResultEnum finalizeCore();

};

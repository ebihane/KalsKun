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

    static const unsigned char  MELODY_1_TABLE_MAX = 44;
    static const unsigned char  MELODY_2_TABLE_MAX = 164;

    /* �������\���� */
    typedef struct
    {
        float Frequency;    /* ���g�� */
        float Time;         /* �L�[�v���鎞�� (sec) */
    } MelodyLineStr;

    static const MelodyLineStr  MELODY_1_TABLE[MELODY_1_TABLE_MAX];
    static const MelodyLineStr  MELODY_2_TABLE[MELODY_2_TABLE_MAX];

    Stopwatch       m_Watch;
    float           m_ThreshTime;
    MelodyModeEnum  m_Mode;
    unsigned long   m_Index;
    unsigned long   m_IndexMax;

    ResultEnum initializeCore();
    ResultEnum doMainProc();
    ResultEnum finalizeCore();

};

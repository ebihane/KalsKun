#include "Parts/CommanderCommon.h"
#include "BuzzerController.h"

/* ‰¹ŠK - Žü”g” */
#define OFF             (0.0f)
#define LOW_DO          (261.63f)
#define LOW_DO_SHARP    (277.18f)
#define RE              (293.66f)
#define RE_SHARP        (311.13f)
#define MI              (329.63f)
#define FA              (349.23f)
#define FA_SHARP        (369.99f)
#define SO              (392.00f)
#define SO_SHARP        (415.30f)
#define RA              (440.00f)
#define RA_SHARP        (466.16f)
#define SI              (493.88f)
#define HIGH_DO         (523.25f)

/* Melody 1 : ƒhƒŒƒ~‚Ì‰Ì c ‚Ì‚Í‚¸ */
const BuzzerController::MelodyLineStr BuzzerController::MELODY_1_TABLE[44]
= {
    { LOW_DO, 0.3f }, { RE,  0.3f }, { MI,     0.3f }, { FA,  0.3f }, { MI,     0.3f }, { RE,  0.3f }, { LOW_DO, 0.8f }, { OFF, 0.2f },
    { MI,     0.3f }, { FA,  0.3f }, { SO,     0.3f }, { RA,  0.3f }, { SO,     0.3f }, { FA,  0.3f }, { MI,     0.8f }, { OFF, 0.2f },
    { LOW_DO, 0.5f }, { OFF, 0.2f }, { LOW_DO, 0.5f }, { OFF, 0.2f }, { LOW_DO, 0.5f }, { OFF, 0.2f }, { LOW_DO, 0.5f }, { OFF, 0.2f },
    { LOW_DO, 0.2f }, { OFF, 0.1f }, { LOW_DO, 0.2f }, { OFF, 0.1f }, { RE,     0.2f }, { OFF, 0.1f }, { RE,     0.2f }, { OFF, 0.1f },
    { MI,     0.2f }, { OFF, 0.1f }, { MI,     0.2f }, { OFF, 0.1f }, { FA,     0.2f }, { OFF, 0.1f }, { FA,     0.2f }, { OFF, 0.1f },
    { MI,     0.3f }, { RE,  0.3f }, { LOW_DO, 1.0f }, { OFF, 1.0f },
};

BuzzerController::BuzzerController()
 : LoopThreadBase((char*)"BuzzerCont", 100, TypeEnum::TIMER_STOP)
 , m_Mode(MelodyModeEnum::E_MELODY_SILENT)
 , m_Index(0)
{
    /* nop. */
}

BuzzerController::~BuzzerController()
{
    /* nop. */
}

ResultEnum BuzzerController::initializeCore()
{
    softToneCreate(IO_BUZZER);
    softToneWrite(IO_BUZZER, (int)OFF);
    m_Mode = E_MELODY_SILENT;
    return ResultEnum::NormalEnd;
}

ResultEnum BuzzerController::doMainProc()
{
    if (pShareMemory->Commander.MelodyMode != m_Mode)
    {
        m_Index = 0;
        m_Mode = pShareMemory->Commander.MelodyMode;

        /* •Ï‰»‚µ‚½‚Æ‚«‚Í300msecŠÔ‚ð‚ ‚¯‚é */
        delay(300);

        if (m_Mode == MelodyModeEnum::E_MELODY_SOUND_1)
        {
            softToneWrite(IO_BUZZER, (int)MELODY_1_TABLE[m_Index].Frequency);
            m_Watch.Start();
        }
    }

    if (m_Mode == MelodyModeEnum::E_MELODY_SOUND_1)
    {
        float elapsed = m_Watch.GetSplit();
        if (MELODY_1_TABLE[m_Index].Time <= elapsed)
        {
            m_Watch.Stop();
            m_Index++;
            if (44 <= m_Index)
            {
                m_Index = 0;
            }

            softToneWrite(IO_BUZZER, (int)MELODY_1_TABLE[m_Index].Frequency);
            m_Watch.Start();
        }
    }
    else if (m_Mode == MelodyModeEnum::E_MELODY_WARNING)
    {
        softToneWrite(IO_BUZZER, 500);
    }
    else if (m_Mode == MelodyModeEnum::E_MELODY_ERROR)
    {
        softToneWrite(IO_BUZZER, 500);
    }
    else
    {
        softToneWrite(IO_BUZZER, 0);
    }

    return ResultEnum::NormalEnd;
}

ResultEnum BuzzerController::finalizeCore()
{
    softToneStop(IO_BUZZER);
    return ResultEnum::NormalEnd;
}
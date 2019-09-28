#include "Parts/CommanderCommon.h"
#include "BuzzerController.h"

/* 音階 - 周波数 */
#define OFF             (0.0f)

/* 基本音階 */
#define DO              (261.63f)
#define DO_SHARP        (277.18f)
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

/* 1 オクターブ下 */
#define LOW_DO          (DO / 2)
#define LOW_DO_SHARP    (DO_SHARP / 2)
#define LOW_RE          (RE / 2)
#define LOW_RE_SHARP    (RE_SHARP / 2)
#define LOW_MI          (MI / 2)
#define LOW_FA          (FA / 2)
#define LOW_FA_SHARP    (FA_SHARP / 2)
#define LOW_SO          (SO / 2)
#define LOW_SO_SHARP    (SO_SHARP / 2)
#define LOW_RA          (RA / 2)
#define LOW_RA_SHARP    (RA_SHARP / 2)
#define LOW_SI          (SI / 2)

/* 1 オクターブ 上 */
#define HIGH_DO         (DO * 2)
#define HIGH_DO_SHARP   (DO_SHARP * 2)
#define HIGH_RE         (RE * 2)
#define HIGH_RE_SHARP   (RE_SHARP * 2)
#define HIGH_MI         (MI * 2)
#define HIGH_FA         (FA * 2)
#define HIGH_FA_SHARP   (FA_SHARP * 2)
#define HIGH_SO         (SO * 2)
#define HIGH_SO_SHARP   (SO_SHARP * 2)
#define HIGH_RA         (RA * 2)
#define HIGH_RA_SHARP   (RA_SHARP * 2)
#define HIGH_SI         (SI * 2)


/* Melody 1 : カエルの歌 */
const BuzzerController::MelodyLineStr BuzzerController::MELODY_1_TABLE[44]
= {
    { HIGH_DO,  0.5f }, { HIGH_RE,  0.5f }, { HIGH_MI,  0.5f }, { HIGH_FA,  0.5f },
    { HIGH_MI,  0.5f }, { HIGH_RE,  0.5f }, { HIGH_DO,  0.5f }, { OFF,      0.5f },
    { HIGH_MI,  0.5f }, { HIGH_FA,  0.5f }, { HIGH_SO,  0.5f }, { HIGH_RA,  0.5f },
    { HIGH_SO,  0.5f }, { HIGH_FA,  0.5f }, { HIGH_MI,  0.5f }, { OFF,      0.5f },
    { HIGH_DO,  0.5f }, { OFF,      0.5f }, { HIGH_DO,  0.5f }, { OFF,      0.5f },
    { HIGH_DO,  0.5f }, { OFF,      0.5f }, { HIGH_DO,  0.5f }, { OFF,      0.5f },
    { HIGH_DO,  0.2f }, { OFF,      0.0f }, { HIGH_DO,  0.2f }, { OFF,      0.0f },
    { HIGH_RE,  0.2f }, { OFF,      0.0f }, { HIGH_RE,  0.2f }, { OFF,      0.0f },
    { HIGH_MI,  0.2f }, { OFF,      0.0f }, { HIGH_MI,  0.2f }, { OFF,      0.0f },
    { HIGH_FA,  0.2f }, { OFF,      0.0f }, { HIGH_FA,  0.2f }, { OFF,      0.0f },
    { HIGH_MI,  0.5f }, { HIGH_RE,  0.5f }, { HIGH_DO,  0.8f }, { OFF,      1.0f },
};

#if 0
const BuzzerController::MelodyLineStr BuzzerController::MELODY_2_TABLE[120]
= {

};
#endif

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

        /* 変化したときは300msec間をあける */
        delay(300);

        if (m_Mode == MelodyModeEnum::E_MELODY_SOUND_1)
        {
            softToneWrite(IO_BUZZER, (int)MELODY_1_TABLE[0].Frequency);
            m_ThreshTime = MELODY_1_TABLE[0].Time;
            m_IndexMax = 44;
        }
        else if (m_Mode == MelodyModeEnum::E_MELODY_WARNING)
        {
            softToneWrite(IO_BUZZER, (int)HIGH_RA);
            m_ThreshTime = 0.5f;
            m_IndexMax = 2;
        }
        else if (m_Mode == MelodyModeEnum::E_MELODY_ERROR)
        {
            softToneWrite(IO_BUZZER, (int)(HIGH_DO * 2));
            m_ThreshTime = 0.3f;
            m_IndexMax = 2;
        }
        else
        {
            m_IndexMax = 0;
        }

        m_Watch.Start();
    }

    if (m_Mode == MelodyModeEnum::E_MELODY_SILENT)
    {
        softToneWrite(IO_BUZZER, OFF);
    }
    else if (m_Watch.GetSplit() <= m_ThreshTime)
    {
        /* nop. */
    }
    else
    {
        m_Watch.Stop();

        m_Index++;
        if (m_IndexMax <= m_Index)
        {
            m_Index = 0;
        }

        if (m_Mode == E_MELODY_WARNING)
        {
            if (m_Index == 0)
            {
                softToneWrite(IO_BUZZER, (int)HIGH_RA);
            }
            else
            {
                softToneWrite(IO_BUZZER, OFF);
            }
        }
        else if (m_Mode == E_MELODY_ERROR)
        {
            if (m_Index == 0)
            {
                softToneWrite(IO_BUZZER, (int)(HIGH_DO * 2));
            }
            else
            {
                softToneWrite(IO_BUZZER, OFF);
            }
        }
        else if (m_Mode == E_MELODY_SOUND_1)
        {
            m_ThreshTime = MELODY_1_TABLE[m_Index].Time;
            softToneWrite(IO_BUZZER, (int)MELODY_1_TABLE[m_Index].Frequency);
        }
        else
        {
            softToneWrite(IO_BUZZER, OFF);
        }

        m_Watch.Start();
    }

    return ResultEnum::NormalEnd;
}

ResultEnum BuzzerController::finalizeCore()
{
    softToneStop(IO_BUZZER);
    return ResultEnum::NormalEnd;
}
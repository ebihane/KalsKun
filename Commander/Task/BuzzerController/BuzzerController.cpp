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
const BuzzerController::MelodyLineStr BuzzerController::MELODY_1_TABLE[MELODY_1_TABLE_MAX]
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

/* Melody 2 : 社歌 */
const BuzzerController::MelodyLineStr BuzzerController::MELODY_2_TABLE[MELODY_2_TABLE_MAX]
= {
    /* 1 小節 : 1 (1) */
    { LOW_SI,   0.2f },

    /* 2 小節 : 8 (9) */
    { RA,  0.6f }, { OFF,   0.1f }, { RA,  0.2f }, { OFF, 0.1f }, { RA,    0.4f }, { OFF,    0.1f }, { MI,  0.2f }, { FA,  0.2f },

    /* 3 小節 : 7 (16) */
    { SO,  0.6f }, { OFF,   0.1f }, { SO,  0.2f }, { OFF, 0.1f }, { SO,    0.4f }, { RA,  0.2f }, { SO,  0.2f },

    /* 4 小節 : 6 (22) */
    { FA,  0.6f }, { OFF,   0.1f }, { FA,  0.2f }, { OFF, 0.1f }, { FA,  0.4f }, { SO,  0.4f },

    /* 5 小節 : 2 (24) */
    { RE_SHARP, 1.6f }, { OFF, 0.1f },

    /* 6 小節 : 5 (29) */
    { RE,  0.4f }, { DO_SHARP, 0.2f }, { RE,  0.2f }, { HIGH_DO,  0.6f }, { RA_SHARP,  0.2f },

    /* 7 小節 : 5 (34) */
    { RA,  1.2f }, { RE,  0.2f }, { OFF,      0.1f }, { RE,  0.2f }, { OFF,      0.1f },

    /* 8 小節 : 5 (39) */
    { RE,  0.4f }, { MI,  0.4f }, { RA,  0.2f }, { SO,  0.4f }, { FA,  0.2f },

    /* 9 小節 : 2 (41) */
    { SO,  1.6f }, { OFF, 0.1f },

    /* 10 小節 : 1 (42) */
    { LOW_SI,   0.2f },

    /* 11 小節 : 8 (50) */
    { RA,  0.6f }, { OFF,   0.1f }, { RA,  0.2f }, { OFF,      0.1f }, { RA,  0.4f }, { OFF,      0.1f }, { MI,  0.2f }, { FA,  0.2f },

    /* 12 小節 : 7 (57) */
    { SO,  0.6f }, { OFF,   0.1f }, { SO,  0.2f }, { OFF,      0.1f }, { SO,  0.4f }, { RA,  0.2f }, { SO,  0.2f },

    /* 13 小節 : 6 (63) */
    { FA,  0.6f }, { OFF,   0.1f }, { FA,  0.2f }, { OFF,      0.1f }, { FA,  0.4f }, { SO,  0.4f },

    /* 14 小節 : 2 (65) */
    { RE_SHARP, 1.6f }, { OFF, 0.1f },

    /* 15 小節 :  (72) */
    { RE,  0.4f }, { DO_SHARP, 0.2f }, { RE,  0.2f }, { HIGH_DO,  0.2f }, { OFF, 0.1f }, { HIGH_DO,  0.4f }, { RA_SHARP,  0.2f },

    /* 16 小節 : 5 (77) */
    { RA,  1.2f }, { RE,    0.2f }, { OFF,      0.1f }, { RE,  0.2f }, { OFF,      0.1f },

    /* 17 小節 : 8 (85) */
    { RE,  0.4f }, { OFF,   0.1f }, { RE,  0.2f }, { RA,  0.2f }, { SO,  0.2f }, { FA,  0.2f }, { MI,  0.2f },{ SO,  0.2f },

    /* 18 小節 : 2 (87) */
    { FA,  1.6f }, { OFF,   0.1f },

    /* 19 小節 : 6 (93) */
    { FA,  0.2f }, { HIGH_RE, 0.6f }, { OFF, 0.1f }, { HIGH_RE, 0.2f }, { HIGH_MI, 0.6f }, { HIGH_RE, 0.2f },
    
    /* 20 小節 : 4 (97) */
    { HIGH_DO,  0.8f }, { HIGH_RE,   0.4f }, { HIGH_DO,  0.2f }, { RA, 0.2f },

    /* 21 小節 : 4 (101) */
    { SO,  0.4f }, { RA, 0.4f}, { RA_SHARP, 0.6f }, { SO, 0.2f },
    
    /* 22 小節 : 2 (103) */
    { HIGH_DO, 1.6f }, { OFF, 0.1f },

    /* 23 小節 : 7 (110) */
    { FA,  0.2f }, { HIGH_RE, 0.6f }, { OFF, 0.1f }, { HIGH_RE, 0.2f }, { HIGH_MI, 0.2f }, { HIGH_RE, 0.2f }, { HIGH_MI, 0.2f },

    /* 24 小節 : 4 (114) */
    { HIGH_FA, 1.4f }, { OFF, 0.1f }, { HIGH_RE, 0.2f }, { HIGH_FA, 0.2f },

    /* 25 小節 : 5 (119) */
    { HIGH_MI, 0.4f }, { HIGH_DO, 0.4f }, { HIGH_RE, 0.2f }, { HIGH_DO, 0.4f }, { SI, 0.2f },

    /* 26 小節 : 2 (121) */
    { HIGH_DO, 1.6f }, { OFF, 0.1f },

    /* 27 小節 : 1 (122) */
    { LOW_SI,   0.2f },

    /* 28 小節 : 8 (130) */
    { RA,  0.6f }, { OFF,   0.1f }, { RA,  0.2f }, { OFF, 0.1f }, { RA,    0.4f }, { OFF,    0.1f }, { MI,  0.2f }, { FA,  0.2f },

    /* 29 小節 : 7 (137) */
    { SO,  0.6f }, { OFF,   0.1f }, { SO,  0.2f }, { OFF, 0.1f }, { SO,    0.4f }, { RA,  0.2f }, { SO,  0.2f },

    /* 30 小節 : 6 (143) */
    { FA,  0.6f }, { OFF,   0.1f }, { FA,  0.2f }, { OFF, 0.1f }, { FA,  0.4f }, { SO,  0.4f },

    /* 31 小節 : 2 (145) */
    { RE_SHARP, 1.6f }, { OFF, 0.1f },

    /* 32 小節 : 5 (150) */
    { RE,  0.4f }, { DO_SHARP, 0.2f }, { RE,  0.2f }, { HIGH_DO,  0.6f }, { RA_SHARP,  0.2f },

    /* 33 小節 : 6 (156) */
    { RA,  0.3f }, { OFF, 0.1f}, { RA, 0.2f }, { HIGH_DO, 0.2f }, { FA, 1.2f }, { OFF, 0.1f },
    
    /* 34 小節 : 6 (162) */
    { RE,  0.2f }, { FA, 0.2f }, { RA, 0.2f }, { SO, 0.2f }, { FA, 0.2f }, { SO, 0.2f },

    /* 35 小節 : 2 (164)  */
    { FA, 1.6f }, { OFF, 1.0f },
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

        /* 変化したときは300msec間をあける */
        delay(300);

        if (m_Mode == MelodyModeEnum::E_MELODY_SOUND_1)
        {
            softToneWrite(IO_BUZZER, (int)MELODY_1_TABLE[0].Frequency);
            m_ThreshTime = MELODY_1_TABLE[0].Time;
            m_IndexMax = MELODY_1_TABLE_MAX;
        }
        else if (m_Mode == MelodyModeEnum::E_MELODY_SOUND_2)
        {
            softToneWrite(IO_BUZZER, (int)MELODY_2_TABLE[0].Frequency);
            m_ThreshTime = MELODY_2_TABLE[0].Time;
            m_IndexMax = MELODY_2_TABLE_MAX;
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
        else if (m_Mode == E_MELODY_SOUND_2)
        {
            m_ThreshTime = MELODY_2_TABLE[m_Index].Time;
            softToneWrite(IO_BUZZER, (int)MELODY_2_TABLE[m_Index].Frequency);
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
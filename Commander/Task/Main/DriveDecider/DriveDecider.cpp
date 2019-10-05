#include <stdio.h>
#include "Include/Common.h"
#include "DriveDecider.h"

DriveDecider::DriveDecider()
 : m_Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::BOTH_OUT)
 , m_AreaMap(NULL)
 , m_MoveMap(NULL)
 , m_Position(NULL)
 , m_Setting(NULL)
 , m_Preview(MotorCommandEnum::E_COMMAND_STOP)
 , m_TurnCount(0)
{
    m_AreaMap = AreaMap::GetInstance();
    m_MoveMap = MoveMap::GetInstance();
    m_Position = PositionData::GetInstance();
    m_Setting = SettingManager::GetInstance();
    m_Setting->GetMapCount(&m_MapCount);
}

DriveDecider::~DriveDecider()
{
    /* nop. */
}

MotorCommandEnum DriveDecider::Decide()
{
    MotorCommandEnum retVal = MotorCommandEnum::E_COMMAND_STOP;

#ifdef TAPE_COUNT_EXECUTE
    retVal = MotorCommandEnum::E_COMMAND_FRONT;
#else
    /* ���̍��W���擾 */
    RectStr nextPosition;
    m_Position->GetNextPosition(&nextPosition);

    /* ��{�͑O�i�F�O���ɐi�߂�ꍇ */
    bool areaMovable = m_AreaMap->IsMovable(&nextPosition);
    bool moveMovable = m_MoveMap->IsMovable(&nextPosition);
    if ((areaMovable == true) && (moveMovable == true))
    {
        retVal = MotorCommandEnum::E_COMMAND_FRONT;
    }
    /* �[�ɓ��B�������� */
    else if (m_FrontKeepWatch.IsRunninng() == false)
    {
        /* ���������O�i�����邽�߁A�X�g�b�v�E�H�b�`�N�� */
        m_FrontKeepWatch.Start();
        retVal = MotorCommandEnum::E_COMMAND_FRONT;
    }
    else if (m_FrontKeepWatch.GetSplit() <= 1.0f)
    {
        /* �[�ɓ��B���Ă��� 1 �b�Ԃ͑O�ɐi�� (�Ƃ肠�������ߑł�) */
        retVal = MotorCommandEnum::E_COMMAND_FRONT;
    }
    else
    {
        m_FrontKeepWatch.Stop();

        RectStr leftPosition;
        RectStr rightPosition;

        m_Position->GetNextPosition(DirectionEnum::E_DIR_LEFT, &leftPosition);
        m_Position->GetNextPosition(DirectionEnum::E_DIR_RIGHT, &rightPosition);

        /* �܂��� AreaMap �Ŕ��f */
        bool leftMovable = m_AreaMap->IsMovable(&leftPosition);
        bool rightMovable = m_AreaMap->IsMovable(&rightPosition);

        /* �ǂ�����s���Ȃ��ꍇ */
        if ((leftMovable == false) && (rightMovable == false))
        {
            /* �Ƃ肠�����E�^�[���Ŗ߂� */
            retVal = MotorCommandEnum::E_COMMAND_R_TURN;
        }
        /* �������s����ꍇ */
        else if ((leftMovable == true) && (rightMovable == false))
        {
            /* ���^�[�� */
            retVal = MotorCommandEnum::E_COMMAND_L_TURN;
        }
        /* �E�����s����ꍇ */
        else if ((leftMovable == false) && (rightMovable == true))
        {
            /* �E�^�[�� */
            retVal = MotorCommandEnum::E_COMMAND_R_TURN;
        }
        /* �����s����ꍇ */
        else
        {
            /* MoveMap �Ŕ��f */
            bool leftNotMoved = m_MoveMap->IsNotMove(&leftPosition);
            bool rightNotMoved = m_MoveMap->IsNotMove(&rightPosition);

            /* ���������ړ� */
            if ((leftNotMoved == true) && (rightNotMoved == false))
            {
                /* ���^�[�� */
                retVal = MotorCommandEnum::E_COMMAND_L_TURN;
            }
            /* �E�������ړ� */
            else if ((leftNotMoved == false) && (rightNotMoved == true))
            {
                /* �E�^�[�� */
                retVal = MotorCommandEnum::E_COMMAND_R_TURN;
            }
            /* �����Ƃ����ړ� */
            /* �������́A�����Ƃ��ړ��ς� */
            else
            {
                /* ���ړ��G���A���������ɋȂ��� */
                if (m_Position->JudgeNotMoveArea() == DirectionEnum::E_DIR_RIGHT)
                {
                    retVal = MotorCommandEnum::E_COMMAND_R_TURN;
                }
                else
                {
                    retVal = MotorCommandEnum::E_COMMAND_L_TURN;
                }
            }
        }
    }
#endif

    /* �^�[���̏ꍇ�͌����Ă�������𔽓] */
    if ((retVal == MotorCommandEnum::E_COMMAND_L_TURN)
    ||  (retVal == MotorCommandEnum::E_COMMAND_R_TURN))
    {
        m_Position->SetDirection(DirectionEnum::E_DIR_BACK);
    }

    m_Preview = retVal;

    return retVal;
}

MotorCommandEnum DriveDecider::DecideForRoadClosed()
{
    MotorCommandEnum retVal = MotorCommandEnum::E_COMMAND_STOP;

#ifdef TAPE_COUNT_EXECUTE
    if ((m_Preview != MotorCommandEnum::E_COMMAND_L_TURN)
    &&  (m_Preview != MotorCommandEnum::E_COMMAND_R_TURN))
    {
        m_TurnCount++;
        char logStr[LOG_OUT_MAX] = { 0 };
        snprintf(&logStr[0], sizeof(logStr), "[Kusakari] Turn : %ld\n", m_TurnCount);
        m_Logger.LOG_INFO(logStr);
    }

    if ((m_TurnCount % 2) == 0)
    {
        retVal = MotorCommandEnum::E_COMMAND_L_TURN;
    }
    else
    {
        retVal = MotorCommandEnum::E_COMMAND_R_TURN;
    }
#else

    /* �����Ă�������̐�̃f�[�^�����ׂāu�ʍs�֎~�v�ɂ��� */
/*     m_Position->FillAreaMap(AreaMap::NOT_MOVABLE_VALUE); */

    RectStr leftPosition;
    RectStr rightPosition;

    m_Position->GetNextPosition(DirectionEnum::E_DIR_LEFT, &leftPosition);
    m_Position->GetNextPosition(DirectionEnum::E_DIR_RIGHT, &rightPosition);

    /* �܂��� AreaMap �Ŕ��f */
    bool leftMovable = m_AreaMap->IsMovable(&leftPosition);
    bool rightMovable = m_AreaMap->IsMovable(&rightPosition);

    /* �ǂ�����s���Ȃ��ꍇ */
    if ((leftMovable == false) && (rightMovable == false))
    {
        /* �Ƃ肠�����E�^�[���Ŗ߂� */
        retVal = MotorCommandEnum::E_COMMAND_R_TURN;
    }
    /* �������s����ꍇ */
    else if ((leftMovable == true) && (rightMovable == false))
    {
        /* ���^�[�� */
        retVal = MotorCommandEnum::E_COMMAND_L_TURN;
        m_Logger.LOG_INFO("[DecideForRoadClosed] L Turn (L only movable)\n");
    }
    /* �E�����s����ꍇ */
    else if ((leftMovable == false) && (rightMovable == true))
    {
        /* �E�^�[�� */
        retVal = MotorCommandEnum::E_COMMAND_R_TURN;
        m_Logger.LOG_INFO("[DecideForRoadClosed] R Turn (R only movable)\n");
    }
    /* �����s����ꍇ */
    else
    {
        /* MoveMap �Ŕ��f */
        bool leftNotMoved = m_MoveMap->IsNotMove(&leftPosition);
        bool rightNotMoved = m_MoveMap->IsNotMove(&rightPosition);

        /* ���������ړ� */
        if ((leftNotMoved == true) && (rightNotMoved == false))
        {
            /* ���^�[�� */
            retVal = MotorCommandEnum::E_COMMAND_L_TURN;
            m_Logger.LOG_INFO("[DecideForRoadClosed] L Turn (L only not moved)\n");
        }
        /* �E�������ړ� */
        else if ((leftNotMoved == false) && (rightNotMoved == true))
        {
            /* �E�^�[�� */
            retVal = MotorCommandEnum::E_COMMAND_R_TURN;
            m_Logger.LOG_INFO("[DecideForRoadClosed] R Turn (R only not moved)\n");
        }
        /* �����Ƃ����ړ� */
        /* �������́A�����Ƃ��ړ��ς� */
        else
        {
            /* ���ړ��G���A���������ɋȂ��� */
            if (m_Position->JudgeNotMoveArea() == DirectionEnum::E_DIR_RIGHT)
            {
                m_Logger.LOG_INFO("[DecideForRoadClosed] R Turn (R area)\n");
                retVal = MotorCommandEnum::E_COMMAND_R_TURN;
            }
            else
            {
                m_Logger.LOG_INFO("[DecideForRoadClosed] L Turn (L area)\n");
                retVal = MotorCommandEnum::E_COMMAND_L_TURN;
            }
        }
    }
#endif

    /* �^�[���̏ꍇ�͌����Ă�������𔽓] */
    if ((retVal == MotorCommandEnum::E_COMMAND_L_TURN)
    ||  (retVal == MotorCommandEnum::E_COMMAND_R_TURN))
    {
        m_Position->SetDirection(DirectionEnum::E_DIR_BACK);
    }

    m_Preview = retVal;

    return retVal;
}

MotorCommandEnum DriveDecider::DecideForAvoidance()
{
    m_Preview = MotorCommandEnum::E_COMMAND_AVOID;
    return MotorCommandEnum::E_COMMAND_AVOID;
}

/* �ԗ��������� */
bool DriveDecider::IsComplete()
{
    bool retVal = false;
    RectStr current = m_Position->GetPosition();

    /* ���̋��ɓ��B������ */
    /* @todo : �����`�̔� */
    if (((current.X == 1) && (current.Y == 1))
    ||  ((current.X == 1) && (current.Y == (m_MapCount.Y - 2)))
    ||  ((current.X == (m_MapCount.X - 2)) && (current.Y == 1))
    ||  ((current.X == (m_MapCount.X - 2)) && (current.Y == (m_MapCount.Y - 2))))
    {
        /* �S�̃G���A�� 95 % �𑖔j�ς� */
        if (m_MoveMap->IsComplete() == true)
        {
            retVal = true;
        }
    }

    return retVal;
}

unsigned long DriveDecider::GetTurnCount()
{
    return m_TurnCount;
}

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
    /* 次の座標を取得 */
    RectStr nextPosition;
    m_Position->GetNextPosition(&nextPosition);

    /* 基本は前進：前方に進める場合 */
    bool areaMovable = m_AreaMap->IsMovable(&nextPosition);
    bool moveMovable = m_MoveMap->IsMovable(&nextPosition);
    if ((areaMovable == true) && (moveMovable == true))
    {
        retVal = MotorCommandEnum::E_COMMAND_FRONT;
    }
    /* 端に到達した直後 */
    else if (m_FrontKeepWatch.IsRunninng() == false)
    {
        /* 少しだけ前進させるため、ストップウォッチ起動 */
        m_FrontKeepWatch.Start();
        retVal = MotorCommandEnum::E_COMMAND_FRONT;
    }
    else if (m_FrontKeepWatch.GetSplit() <= 1.0f)
    {
        /* 端に到達してから 1 秒間は前に進む (とりあえず決め打ち) */
        retVal = MotorCommandEnum::E_COMMAND_FRONT;
    }
    else
    {
        m_FrontKeepWatch.Stop();

        RectStr leftPosition;
        RectStr rightPosition;

        m_Position->GetNextPosition(DirectionEnum::E_DIR_LEFT, &leftPosition);
        m_Position->GetNextPosition(DirectionEnum::E_DIR_RIGHT, &rightPosition);

        /* まずは AreaMap で判断 */
        bool leftMovable = m_AreaMap->IsMovable(&leftPosition);
        bool rightMovable = m_AreaMap->IsMovable(&rightPosition);

        /* どちらも行けない場合 */
        if ((leftMovable == false) && (rightMovable == false))
        {
            /* とりあえず右ターンで戻る */
            retVal = MotorCommandEnum::E_COMMAND_R_TURN;
        }
        /* 左だけ行ける場合 */
        else if ((leftMovable == true) && (rightMovable == false))
        {
            /* 左ターン */
            retVal = MotorCommandEnum::E_COMMAND_L_TURN;
        }
        /* 右だけ行ける場合 */
        else if ((leftMovable == false) && (rightMovable == true))
        {
            /* 右ターン */
            retVal = MotorCommandEnum::E_COMMAND_R_TURN;
        }
        /* 両方行ける場合 */
        else
        {
            /* MoveMap で判断 */
            bool leftNotMoved = m_MoveMap->IsNotMove(&leftPosition);
            bool rightNotMoved = m_MoveMap->IsNotMove(&rightPosition);

            /* 左側が未移動 */
            if ((leftNotMoved == true) && (rightNotMoved == false))
            {
                /* 左ターン */
                retVal = MotorCommandEnum::E_COMMAND_L_TURN;
            }
            /* 右側が未移動 */
            else if ((leftNotMoved == false) && (rightNotMoved == true))
            {
                /* 右ターン */
                retVal = MotorCommandEnum::E_COMMAND_R_TURN;
            }
            /* 両方とも未移動 */
            /* もしくは、両方とも移動済み */
            else
            {
                /* 未移動エリアが多い方に曲がる */
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

    /* ターンの場合は向いている方向を反転 */
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

    /* 向いている方向の先のデータをすべて「通行禁止」にする */
/*     m_Position->FillAreaMap(AreaMap::NOT_MOVABLE_VALUE); */

    RectStr leftPosition;
    RectStr rightPosition;

    m_Position->GetNextPosition(DirectionEnum::E_DIR_LEFT, &leftPosition);
    m_Position->GetNextPosition(DirectionEnum::E_DIR_RIGHT, &rightPosition);

    /* まずは AreaMap で判断 */
    bool leftMovable = m_AreaMap->IsMovable(&leftPosition);
    bool rightMovable = m_AreaMap->IsMovable(&rightPosition);

    /* どちらも行けない場合 */
    if ((leftMovable == false) && (rightMovable == false))
    {
        /* とりあえず右ターンで戻る */
        retVal = MotorCommandEnum::E_COMMAND_R_TURN;
    }
    /* 左だけ行ける場合 */
    else if ((leftMovable == true) && (rightMovable == false))
    {
        /* 左ターン */
        retVal = MotorCommandEnum::E_COMMAND_L_TURN;
        m_Logger.LOG_INFO("[DecideForRoadClosed] L Turn (L only movable)\n");
    }
    /* 右だけ行ける場合 */
    else if ((leftMovable == false) && (rightMovable == true))
    {
        /* 右ターン */
        retVal = MotorCommandEnum::E_COMMAND_R_TURN;
        m_Logger.LOG_INFO("[DecideForRoadClosed] R Turn (R only movable)\n");
    }
    /* 両方行ける場合 */
    else
    {
        /* MoveMap で判断 */
        bool leftNotMoved = m_MoveMap->IsNotMove(&leftPosition);
        bool rightNotMoved = m_MoveMap->IsNotMove(&rightPosition);

        /* 左側が未移動 */
        if ((leftNotMoved == true) && (rightNotMoved == false))
        {
            /* 左ターン */
            retVal = MotorCommandEnum::E_COMMAND_L_TURN;
            m_Logger.LOG_INFO("[DecideForRoadClosed] L Turn (L only not moved)\n");
        }
        /* 右側が未移動 */
        else if ((leftNotMoved == false) && (rightNotMoved == true))
        {
            /* 右ターン */
            retVal = MotorCommandEnum::E_COMMAND_R_TURN;
            m_Logger.LOG_INFO("[DecideForRoadClosed] R Turn (R only not moved)\n");
        }
        /* 両方とも未移動 */
        /* もしくは、両方とも移動済み */
        else
        {
            /* 未移動エリアが多い方に曲がる */
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

    /* ターンの場合は向いている方向を反転 */
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

/* 網羅完了判定 */
bool DriveDecider::IsComplete()
{
    bool retVal = false;
    RectStr current = m_Position->GetPosition();

    /* 畑の隅に到達した時 */
    /* @todo : 正方形の畑 */
    if (((current.X == 1) && (current.Y == 1))
    ||  ((current.X == 1) && (current.Y == (m_MapCount.Y - 2)))
    ||  ((current.X == (m_MapCount.X - 2)) && (current.Y == 1))
    ||  ((current.X == (m_MapCount.X - 2)) && (current.Y == (m_MapCount.Y - 2))))
    {
        /* 全体エリアの 95 % を走破済み */
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

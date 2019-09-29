#include "Parts/Setting/SettingManager.h"
#include "MotorSimulator.h"

MotorSimulator::MotorSimulator()
 : AdapterBase()
 , m_X_Direciton(1)
 , m_Y_Direction(1)
{
    SettingManager* setting = SettingManager::GetInstance();
    setting->GetRobotSize(&m_RobotSize);
}

MotorSimulator::~MotorSimulator()
{
    /* nop. */
}

ResultEnum MotorSimulator::Open()
{
    return ResultEnum::NormalEnd;
}

ResultEnum MotorSimulator::Close()
{
    return ResultEnum::NormalEnd;
}

ResultEnum MotorSimulator::Connection()
{
    return ResultEnum::NormalEnd;
}

ResultEnum MotorSimulator::Disconnection()
{
    return ResultEnum::NormalEnd;
}

ResultEnum MotorSimulator::IsSendable(bool& sendable)
{
    sendable = true;
    return ResultEnum::NormalEnd;
}

ResultEnum MotorSimulator::Send(void* const bufferPtr, const unsigned long size)
{
    char* buffer = (char*)bufferPtr;
    long temp = 0;

    MotorCommandEnum command = (MotorCommandEnum)(buffer[2] & 0x0F);

    temp = buffer[2] & 0xF0;
    temp >>= 4;
    temp &= 0x0F;
    m_CutterMove = (CutterDriveEnum)temp;

    switch (command)
    {
        case MotorCommandEnum::E_COMMAND_STOP :
            stopCommand();
            break;

        case MotorCommandEnum::E_COMMAND_FRONT :
            frontCommand();
            break;

        case MotorCommandEnum::E_COMMAND_R_TURN :
            r_turnCommand();
            break;

        case MotorCommandEnum::E_COMMAND_L_TURN :
            l_turnCommand();
            break;

        case MotorCommandEnum::E_COMMAND_AVOID :
            avoidCommand();
            break;

        case MotorCommandEnum::E_COMMAND_REMOTE :
            remoteCommand();
            break;

        case MotorCommandEnum::E_COMMAND_MONITOR :
            monitorCommand();
            break;

        default :
            break;
    }

    return ResultEnum::NormalEnd;
}

ResultEnum MotorSimulator::IsReceivable(bool& receivable)
{
    receivable = true;
    return ResultEnum::NormalEnd;
}

ResultEnum MotorSimulator::Receive(void* const bufferPtr, const unsigned long size)
{
    char* buffer = (char*)bufferPtr;
    long temp = 0;
    long tempX = 0;
    long tempY = 0;


    updateStatus();

    if (size == 1)
    {
        delay(10);
        buffer[0] = 0xFF;
    }
    else
    {
        buffer[0] = 0x01;

        temp = m_CutterMove;
        temp <<= 8;
        temp += m_CurrentCommand;
        buffer[1] = (char)temp;

        /* cm 単位に変換 */
        /* 実際のモータが X 座標を - 方向に進ませるため、符号反転 */
        tempX = (m_CurrentPosition.X / 10) * -1;
        tempY = (m_CurrentPosition.Y / 10);

        temp = tempX & 0x0000FF00;
        temp >>= 8;
        temp &= 0x000000FF;
        buffer[2] = (char)temp;
        buffer[3] = (char)(tempX & 0x000000FF);

        temp = tempY & 0x0000FF00;
        temp >>= 8;
        temp &= 0x000000FF;
        buffer[4] = (char)temp;
        buffer[5] = (char)(tempY & 0x000000FF);
        buffer[6] = 0;
    }

    return ResultEnum::NormalEnd;
}

void MotorSimulator::stopCommand()
{
    m_AvoidWatch.Stop();
    m_RightTurnWatch.Stop();
    m_LeftTurnWatch.Stop();
    m_CurrentCommand = MotorCommandEnum::E_COMMAND_STOP;
}

void MotorSimulator::frontCommand()
{
    if (m_CurrentCommand == MotorCommandEnum::E_COMMAND_STOP)
    {
        m_CurrentCommand = MotorCommandEnum::E_COMMAND_FRONT;
    }
    else if (m_CurrentCommand == MotorCommandEnum::E_COMMAND_L_TURN)
    {
        if (m_LeftTurnWatch.IsRunninng() == false)
        {
            m_CurrentCommand = MotorCommandEnum::E_COMMAND_FRONT;
        }
    }
    else if (m_CurrentCommand == MotorCommandEnum::E_COMMAND_R_TURN)
    {
        if (m_RightTurnWatch.IsRunninng() == false)
        {
            m_CurrentCommand = MotorCommandEnum::E_COMMAND_FRONT;
        }
    }
    else if (m_CurrentCommand == MotorCommandEnum::E_COMMAND_AVOID)
    {
        if (m_AvoidWatch.IsRunninng() == false)
        {
            m_CurrentCommand = MotorCommandEnum::E_COMMAND_FRONT;
        }
    }
    else
    {
        /* nop. */
    }
}

void MotorSimulator::r_turnCommand()
{
    if (m_RightTurnWatch.IsRunninng() == false)
    {
        m_RightTurnWatch.Start();
        m_CurrentCommand = MotorCommandEnum::E_COMMAND_R_TURN;
    }
}

void MotorSimulator::l_turnCommand()
{
    if (m_LeftTurnWatch.IsRunninng() == false)
    {
        m_LeftTurnWatch.Start();
        m_CurrentCommand = MotorCommandEnum::E_COMMAND_L_TURN;
    }
}

void MotorSimulator::avoidCommand()
{
    if (m_AvoidWatch.IsRunninng() == false)
    {
        m_AvoidWatch.Start();
        m_CurrentCommand = MotorCommandEnum::E_COMMAND_AVOID;
    }

}

void MotorSimulator::remoteCommand()
{
    /* nop. */
}

void MotorSimulator::monitorCommand()
{
    /* nop. */
}

void MotorSimulator::updateStatus()
{
    if (m_RightTurnWatch.IsRunninng() == true)
    {
        if (5.0f <= m_RightTurnWatch.GetSplit())
        {
            m_X_Direciton *= -1;
            m_Y_Direction += (long)(m_RobotSize.Width / 2);
            m_RightTurnWatch.Stop();
        }
    }
    else if (m_LeftTurnWatch.IsRunninng() == true)
    {
        if (5.0f <= m_LeftTurnWatch.GetSplit())
        {
            m_X_Direciton *= -1;
            m_Y_Direction += (long)(m_RobotSize.Width / 2);
            m_LeftTurnWatch.Stop();
        }
    }
    else if (m_AvoidWatch.IsRunninng() == true)
    {
        /* nop. */
    }
    else if (m_CurrentCommand == MotorCommandEnum::E_COMMAND_FRONT)
    {
        m_CurrentPosition.X += 10 * m_X_Direciton;
    }
    else
    {

    }
}

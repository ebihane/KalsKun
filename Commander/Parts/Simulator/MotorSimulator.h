#pragma once

#include "Adapter/AdapterBase.h"
#include "Measure/Stopwatch.h"
#include "Parts/ShareMemory/ShareMemory.h"

class MotorSimulator : public AdapterBase
{
public :

    MotorSimulator();
    virtual ~MotorSimulator();

    ResultEnum Open();
    ResultEnum Close();
    ResultEnum Connection();
    ResultEnum Disconnection();

    ResultEnum IsSendable(bool& sendable);
    ResultEnum Send(void* const bufferPtr, const unsigned long size);
    ResultEnum IsReceivable(bool& receivable);
    ResultEnum Receive(void* const bufferPtr, const unsigned long size);

protected :

private :

    RectStr m_CurrentPosition;
    MotorCommandEnum m_CurrentCommand;
    CutterDriveEnum m_CutterMove;
    MotorCommandEnum m_LastTurnCommand;

    Stopwatch m_AvoidWatch;
    Stopwatch m_RightTurnWatch;
    Stopwatch m_LeftTurnWatch;

    long m_X_Direciton;
    long m_Y_Direction;
    SizeStr m_RobotSize;

    bool m_IsAuto;
    int m_PrevLevel;

    void stopCommand();
    void frontCommand();
    void r_turnCommand();
    void l_turnCommand();
    void avoidCommand();
    void remoteCommand();
    void monitorCommand();

    void updateStatus();
};

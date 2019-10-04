#pragma once

#include <stdio.h>
#include "Queue/Queue.h"
#include "Serial/Serial.h"
#include "ThreadBase/LoopThreadBase.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Parts/PositionConverter/ConverterBase.h"

class MotorCommunicator : public LoopThreadBase
{
public :

    MotorCommunicator(AdapterBase* const adapter, ConverterBase* const converter);
    virtual ~MotorCommunicator();

protected :


private :

    Queue* m_Queue;
    AdapterBase* m_Adapter;
    ConverterBase* m_Converter;
    PositionData* m_Position;
    RectStr m_PrevPosition;
    FILE* m_LogFile;
    FILE* m_RobotMoveLogFile;

    bool m_IsOpen;
    bool m_Opened;
    MotorCommandEnum m_MotorCommand;
    CutterDriveEnum m_CutterMode;

    ResultEnum initializeCore();
    ResultEnum doMainProc();
    ResultEnum finalizeCore();

    ResultEnum createSendData(const MotorCommandEnum command, const CutterDriveEnum cutter, char* const buffer);
    ResultEnum analyze(char* const buffer);
    ResultEnum receiveProc(char* const buffer);
    void outputLog(char* const buffer, const long size, const char type);
    void outputRobotMoveLog(const short realPositionX, const short realPositionY);
};

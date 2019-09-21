#pragma once

#include "Queue/Queue.h"
#include "Serial/Serial.h"
#include "ThreadBase/LoopThreadBase.h"
#include "Parts/ShareMemory/ShareMemory.h"

class MotorCommunicator : public LoopThreadBase
{
public :

    MotorCommunicator();
    virtual ~MotorCommunicator();

protected :


private :

    Queue* m_Queue;
    Serial* m_Serial;

    MotorCommandEnum m_MotorCommand;
    CutterDriveEnum m_CutterMode;

    ResultEnum initializeCore();
    ResultEnum doMainProc();
    ResultEnum finalizeCore();

    ResultEnum createSendData(const MotorCommandEnum command, const CutterDriveEnum cutter, char* const buffer);
    ResultEnum analyze(char* const buffer);
};
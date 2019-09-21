#pragma once

#include "ThreadBase/ReceiverThread.h"

class RemoteController : public ReceiverThread
{
public :

    RemoteController(AdapterBase* const adapter);
    virtual ~RemoteController();

protected :

private :

    ResultEnum initializeCore();
    bool isReceiveComplete(char* const buffer, const unsigned long size);
    ResultEnum analyze(char* const buffer);

    void analyzeRemoteCommand(char* const buffer);
    void analyzeSingleMotorDrive(char* const buffer);
    void analyzeModeChange(char* const buffer);
    void analyzeAlarmReset();

};

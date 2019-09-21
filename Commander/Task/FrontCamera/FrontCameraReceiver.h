#pragma once

#include "ThreadBase/ReceiverThread.h"

class FrontCameraReceiver : public ReceiverThread
{
public :

    FrontCameraReceiver(AdapterBase* const adapter);
    virtual ~FrontCameraReceiver();

protected :

private :

    ResultEnum initializeCore();
    bool isReceiveComplete(char* const buffer, const unsigned long size);
    ResultEnum analyze(char* const buffer);

};

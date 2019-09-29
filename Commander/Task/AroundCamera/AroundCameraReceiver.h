#pragma once

#include "ThreadBase/ReceiverThread.h"

class AroundCameraReceiver : public ReceiverThread
{
public :

    AroundCameraReceiver(AdapterBase* const adapter);
    virtual ~AroundCameraReceiver();

protected :

private :

    ResultEnum initializeCore();
    bool isReceiveComplete(char* const buffer, const unsigned long size);
    ResultEnum analyze(char* const buffer);
    unsigned long createResponse(char* const buffer);

};

#pragma once

#include "ThreadBase/ReceiverThread.h"

class AroundCameraReceiver : public ReceiverThread
{
public :

    AroundCameraReceiver();
    virtual ~AroundCameraReceiver();

protected :

private :

    ResultEnum analyze(EventInfo* const ev);

};

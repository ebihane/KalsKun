#pragma once

#include "ThreadBase/ReceiverThread.h"

class FrontCameraReceiver : public ReceiverThread
{
public :

    FrontCameraReceiver(AdapterBase* const adapter);
    virtual ~FrontCameraReceiver();

protected :

private :

    MoveTypeEnum m_PrevMoveType;
    DetectTypeEnum m_PrevRedTape;
    DetectTypeEnum m_PrevBlueObject;

    ResultEnum initializeCore();
    bool isReceiveComplete(char* const buffer, const unsigned long size);
    ResultEnum analyze(char* const buffer);
    unsigned long createResponse(char* const buffer);

};

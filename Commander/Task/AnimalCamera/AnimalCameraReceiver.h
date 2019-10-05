#pragma once

#include "ThreadBase/ReceiverThread.h"

class AnimalCameraReceiver : public ReceiverThread
{
public :

    AnimalCameraReceiver(AdapterBase* const adapter);
    virtual ~AnimalCameraReceiver();

protected :

private :

    DetectTypeEnum m_PrevHuman;
    DetectTypeEnum m_PrevAnimal;

    ResultEnum initializeCore();
    bool isReceiveComplete(char* const buffer, const unsigned long size);
    ResultEnum analyze(char* const buffer);
    unsigned long createResponse(char* const buffer);

};

#pragma once

#include "Adapter/AdapterBase.h"

class FrontCameraSimulator : public AdapterBase
{
public :

    FrontCameraSimulator();
    virtual ~FrontCameraSimulator();

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

    long m_ReceiveCount;

};
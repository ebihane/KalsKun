#pragma once

#include "Adapter/AdapterBase.h"

class AroundCameraSimulator : public AdapterBase
{
public:

    AroundCameraSimulator();
    virtual ~AroundCameraSimulator();

    ResultEnum Open();
    ResultEnum Close();
    ResultEnum Connection();
    ResultEnum Disconnection();

    ResultEnum IsSendable(bool& sendable);
    ResultEnum Send(void* const bufferPtr, const unsigned long size);
    ResultEnum IsReceivable(bool& receivable);
    ResultEnum Receive(void* const bufferPtr, const unsigned long size);

protected:


private:

    long m_ReceiveCount;

};
#pragma once

#include "Adapter/AdapterBase.h"
#include "ThreadBase/ThreadBase.h"

/* ��M���s���X���b�h�̊��ƂȂ�N���X */
/* �����̃g���K��Adapter�ւ̎�M */
class ReceiverThread : public ThreadBase
{
public :

    ReceiverThread(AdapterBase* const adapter);
    virtual ~ReceiverThread();

protected :

    char* m_RecvData;

    virtual ResultEnum initializeCore() = 0;
    virtual void doReconnectInitialize(const bool isFirst);
    virtual ResultEnum doConnectedProc();
    virtual bool isReceiveComplete(char* const buffer, const unsigned long size) = 0;
    virtual ResultEnum analyze(char* const buffer) = 0;
    virtual ResultEnum finalizeCore();

private :

    AdapterBase* m_Adapter;

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

    ResultEnum receive();

};

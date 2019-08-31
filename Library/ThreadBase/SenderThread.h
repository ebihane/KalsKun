#pragma once

#include "Adapter/AdapterBase.h"
#include "Queue/Queue.h"
#include "ThreadBase/ThreadBase.h"

/* ���}�C�R���ւ̑��M���s���X���b�h�̊��ƂȂ�N���X */
/* �����̃g���K�͌p����Ŏ��R�Ɏw��ł��� (createSendData ���\�b�h�� true ��Ԃ��Ƒ��M�����) */
/* �L���[�Ɏ�M�������e�𑼂̃}�C�R���ɓ]������ */
class SenderThread : public ThreadBase
{
public :

    SenderThread(AdapterBase* const adapter);
    virtual ~SenderThread();

protected :

    char* m_SendData;

    virtual ResultEnum initializeCore() = 0;
    virtual void doReconnectInitialize(const bool isFirst);
    virtual ResultEnum doConnectedProc();
    virtual bool createSendData(char* const data, unsigned long* const size) = 0;
    virtual ResultEnum finalizeCore();

private :

    AdapterBase* m_Adapter;

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

};

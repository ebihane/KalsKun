#pragma once

#include "Adapter/AdapterBase.h"
#include "ThreadBase/ThreadBase.h"

/* ��M���s���X���b�h�̊��ƂȂ�N���X */
/* �����̃g���K��Adapter�ւ̎�M */
/* �h����� initializeCore() ���\�b�h �ɂāAm_RecvData �� allocation ���K�v (�ǂ����ł͂Ȃ����Ƃ͏d�X���m�c) */
/* �������K�v�ȏꍇ�A����� m_ResponseData �� allocation �� m_ResponseSize �ւ̎�M�o�b�t�@�T�C�Y������K�v */
class ReceiverThread : public ThreadBase
{
public :

    ReceiverThread(char* const taskName, AdapterBase* const adapter, const bool responseExist);
    virtual ~ReceiverThread();

protected :

    char* m_RecvData;
    char* m_ResponseData;

    virtual ResultEnum initializeCore() = 0;
    virtual void doReconnectInitialize(const bool isFirst);
    virtual ResultEnum doConnectedProc();
    virtual bool isReceiveComplete(char* const buffer, const unsigned long size) = 0;
    virtual ResultEnum analyze(char* const buffer) = 0;
    virtual unsigned long createResponse(char* const buffer);
    virtual ResultEnum finalizeCore();

private :

    const bool RESPONSE_EXIST;

    AdapterBase* m_Adapter;

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

    ResultEnum receive();

};

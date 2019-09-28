#pragma once

#include "Adapter/AdapterBase.h"
#include "ThreadBase/ThreadBase.h"

/* ���M���s���X���b�h�̊��ƂȂ�N���X */
/* �����̃g���K�͌p����Ŏ��R�Ɏw��ł��� (createSendData ���\�b�h�� true ��Ԃ��Ƒ��M�����) */
/* ���̃N���X�͓����Ń��[�v���� (�ŒZ�̏���������100msec) */
/* �h����� initializeCore() ���\�b�h �ɂāAm_SendData �� allocation ���K�v (�ǂ����ł͂Ȃ����Ƃ͏d�X���m�c) */
/* �������K�v�ȏꍇ�A����� m_RecvData �� allocation �� m_RecvSize �ւ̎�M�o�b�t�@�T�C�Y������K�v */
class SenderThread : public ThreadBase
{
public :

    SenderThread(char* const taskName, AdapterBase* const adapter, const bool responseExist);
    virtual ~SenderThread();

protected :

    char* m_SendData;
    char* m_RecvData;
    unsigned long m_RecvSize;

    virtual ResultEnum initializeCore() = 0;
    virtual void doReconnectInitialize(const bool isFirst);
    virtual ResultEnum doConnectedProc();
    virtual bool createSendData(char* const data, unsigned long* const size) = 0;
    virtual void analyzeResponse(char* const data, const unsigned long size);
    virtual ResultEnum finalizeCore();

private :

    const bool RESPONSE_EXIST;

    AdapterBase* m_Adapter;

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

};

#pragma once

#include "Adapter/AdapterBase.h"
#include "Queue/Queue.h"
#include "ThreadBase/ThreadBase.h"

/* ���}�C�R������̎�M���s���X���b�h�̊��ƂȂ�N���X */
/* �����̃g���K�̓\�P�b�g�ւ̎�M */
/* SenderThread �Ƃ͈Ⴂ�A�L���[�͎����Ȃ����߁A�K�v�ł���Όp����ŗp�ӂ��邱�� */
/* �Ȃ��A�ʐM�f�[�^�̓C�x���g�\���݂̂̂Ƃ��Ă��邽�߁A�J�����摜�Ȃǂ̑傫�ȃf�[�^�͎�M�ł��Ȃ� */
/* �ʓr��p�̒ʐM�X���b�h���쐬���邱�� */
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

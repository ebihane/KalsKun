#pragma once

#include "Adapter/AdapterBase.h"
#include "Queue/Queue.h"
#include "ThreadBase/ThreadBase.h"

/* 他マイコンからの受信を行うスレッドの基底となるクラス */
/* 処理のトリガはソケットへの受信 */
/* SenderThread とは違い、キューは持たないため、必要であれば継承先で用意すること */
/* なお、通信データはイベント構造体のみとしているため、カメラ画像などの大きなデータは受信できない */
/* 別途専用の通信スレッドを作成すること */
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

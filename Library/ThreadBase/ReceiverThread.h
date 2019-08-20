#pragma once

#include "Socket/TcpServer/TcpServer.h"
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

    ReceiverThread(const unsigned short portNo);
    virtual ~ReceiverThread();

protected :

    virtual ResultEnum initializeCore();
    virtual void doReconnectInitialize(const bool isFirst);
    virtual ResultEnum doConnectedProc();
    virtual ResultEnum analyze(EventInfo* const ev) = 0;
    virtual ResultEnum finalizeCore();

private :

    TcpServer* m_TcpServer;

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

};

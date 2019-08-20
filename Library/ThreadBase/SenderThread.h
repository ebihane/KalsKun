#pragma once

#include "Socket/TcpClient/TcpClient.h"
#include "Queue/Queue.h"
#include "ThreadBase/ThreadBase.h"

/* 他マイコンへの送信を行うスレッドの基底となるクラス */
/* 処理のトリガは継承先で自由に指定できる (createSendData メソッドで true を返すと送信される) */
/* キューに受信した内容を他のマイコンに転送する */
class SenderThread : public ThreadBase
{
public :

    SenderThread(char* const ipAddress, const unsigned short portNo);
    virtual ~SenderThread();

protected :

    virtual ResultEnum initializeCore();
    virtual void doReconnectInitialize(const bool isFirst);
    virtual ResultEnum doConnectedProc();
    virtual bool createSendData(EventInfo* const ev) = 0;
    virtual ResultEnum finalizeCore();

private :

    TcpClient* m_TcpClient;

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

};

#pragma once

#include "Adapter/AdapterBase.h"
#include "Queue/Queue.h"
#include "ThreadBase/ThreadBase.h"

/* 他マイコンへの送信を行うスレッドの基底となるクラス */
/* 処理のトリガは継承先で自由に指定できる (createSendData メソッドで true を返すと送信される) */
/* キューに受信した内容を他のマイコンに転送する */
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

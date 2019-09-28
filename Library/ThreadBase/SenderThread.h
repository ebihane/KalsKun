#pragma once

#include "Adapter/AdapterBase.h"
#include "ThreadBase/ThreadBase.h"

/* 送信を行うスレッドの基底となるクラス */
/* 処理のトリガは継承先で自由に指定できる (createSendData メソッドで true を返すと送信される) */
/* このクラスは内部でループする (最短の処理周期は100msec) */
/* 派生先の initializeCore() メソッド にて、m_SendData の allocation が必要 (良い作りではないことは重々承知…) */
/* 応答が必要な場合、さらに m_RecvData の allocation と m_RecvSize への受信バッファサイズ代入も必要 */
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

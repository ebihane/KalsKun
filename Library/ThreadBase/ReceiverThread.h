#pragma once

#include "Adapter/AdapterBase.h"
#include "ThreadBase/ThreadBase.h"

/* 受信を行うスレッドの基底となるクラス */
/* 処理のトリガはAdapterへの受信 */
/* 派生先の initializeCore() メソッド にて、m_RecvData の allocation が必要 (良い作りではないことは重々承知…) */
/* 応答が必要な場合、さらに m_ResponseData の allocation と m_ResponseSize への受信バッファサイズ代入も必要 */
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

#include <stdio.h>
#include <stdlib.h>
#include "ReceiverThread.h"

ReceiverThread::ReceiverThread(char* const taskName, AdapterBase* const adapter)
 : ThreadBase(taskName)
 , m_RecvData(NULL)
 , m_Adapter(adapter)
{
    /* nop. */
}

ReceiverThread::~ReceiverThread()
{
    finalize();
}

void ReceiverThread::doReconnectInitialize(const bool isFirst)
{
    /* 継承しない場合は何もしない */
}

ResultEnum ReceiverThread::doConnectedProc()
{
    /* 継承しない場合は何もせず正常を返す */
    return ResultEnum::NormalEnd;
}

ResultEnum ReceiverThread::finalizeCore()
{
    /* 継承しない場合は何もせず正常を返す */
    return ResultEnum::NormalEnd;
}

ResultEnum ReceiverThread::initialize()
{
    ResultEnum  retVal = ResultEnum::AbnormalEnd;
    ResultEnum  result = ResultEnum::AbnormalEnd;
    char logStr[80] = { 0 };

    if (m_Adapter == NULL)
    {
        m_Logger->LOG_ERROR("[initialize] m_Adapter allocation failed.\n");
        goto FINISH;
    }

    if (m_Adapter->Open() != ResultEnum::NormalEnd)
    {
        snprintf(&logStr[0], sizeof(logStr), "[initialize] m_Adapter Open failed. errno[%d]\n", m_Adapter->GetLastError());
        m_Logger->LOG_ERROR(logStr);
        goto FINISH;
    }

    result = initializeCore();
    if (result != ResultEnum::NormalEnd)
    {
        snprintf(&logStr[0], sizeof(logStr), "[initialize] initializeCore failed. result[%d]\n", result);
        m_Logger->LOG_ERROR(logStr);
        goto FINISH;
    }

    if (m_RecvData == NULL)
    {
        m_Logger->LOG_ERROR("[initialize] m_SendData is NULL.\n");
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

ResultEnum ReceiverThread::doProcedure()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    ResultEnum result = ResultEnum::AbnormalEnd;
    bool isFirst = true;
    bool receivable = false;
    char logStr[80] = { 0 };

RECONNECT :

    /* 接続時の初期化処理 */
    doReconnectInitialize(isFirst);
    isFirst = false;

    /* いったん切断する */
    m_Adapter->Disconnection();

    /* 接続 */
    result = m_Adapter->Connection();
    if (result != ResultEnum::NormalEnd)
    {
        if (result == ResultEnum::Reconnect)
        {
            if (isStopRequest() == true)
            {
                goto FINISH;
            }
            else
            {
                delay(3000);
                goto RECONNECT;
            }
        }
        else
        {
            snprintf(&logStr[0], sizeof(logStr), "[doProcedure] Connection failed. errno[%d]\n", m_Adapter->GetLastError());
            m_Logger->LOG_ERROR(logStr);

            goto FINISH;
        }
    }

    snprintf(&logStr[0], sizeof(logStr), "[doProcedure] %s Connection establish.\n", &m_TaskName[0]);
    m_Logger->LOG_INFO(logStr);

    /* 接続確立時処理 */
    result = doConnectedProc();
    if (result != ResultEnum::NormalEnd)
    {
        snprintf(&logStr[0], sizeof(logStr), "[doProcedure] doConnectedProc failed.\n");
        m_Logger->LOG_ERROR(logStr);

        if (result == ResultEnum::Reconnect)
        {
            goto RECONNECT;
        }
        else
        {
            goto FINISH;
        }
    }

    m_Logger->LOG_INFO("[doProcedure] Main loop enter.\n");
    while (1)
    {
        result = m_Adapter->IsReceivable(receivable);
        if (result != ResultEnum::NormalEnd)
        {
            snprintf(&logStr[0], sizeof(logStr), "[doProcedure] IsReceivable failed. errno[%d]\n", m_Adapter->GetLastError());
            m_Logger->LOG_ERROR(logStr);

            if (result == ResultEnum::Reconnect)
            {
                goto RECONNECT;
            }
            else
            {
                goto FINISH;
            }
        }

        if (receivable == false)
        {
            if (isStopRequest() == true)
            {
                m_Logger->LOG_INFO("[doProcedure] Thread stop request.\n");
                break;
            }

            delay(100);
            continue;
        }

        result = receive();
        if (result != ResultEnum::NormalEnd)
        {
            snprintf(&logStr[0], sizeof(logStr), "[doProcedure] Receive failed. errno[%d]\n", m_Adapter->GetLastError());
            m_Logger->LOG_ERROR(logStr);

            if (result == ResultEnum::Reconnect)
            {
                goto RECONNECT;
            }
            else
            {
                goto FINISH;
            }
        }

        /* 受信データの解析 */
        result = analyze(&m_RecvData[0]);
        if (result != ResultEnum::NormalEnd)
        {
            snprintf(&logStr[0], sizeof(logStr), "[doProcedure] analyze failed.\n");
            m_Logger->LOG_ERROR(logStr);

            if (result == ResultEnum::Reconnect)
            {
                goto RECONNECT;
            }
            else
            {
                goto FINISH;
            }
        }
    }
    m_Logger->LOG_INFO("[doProcedure] Main loop exit.\n");

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

ResultEnum ReceiverThread::finalize()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    if (m_Adapter != NULL)
    {
        m_Adapter->Close();
        delete m_Adapter;
        m_Adapter = NULL;
    }

    retVal = finalizeCore();

    return retVal;
}

ResultEnum ReceiverThread::receive()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    ResultEnum result = ResultEnum::AbnormalEnd;
    unsigned long   size = 0;


    while (1)
    {
        result = m_Adapter->Receive(&m_RecvData[size], 1);
        if (result != ResultEnum::NormalEnd)
        {
            retVal = result;
            goto FINISH;
        }

        size++;
        if (isReceiveComplete(&m_RecvData[0], size) == true)
        {
            break;
        }

        delay(10);
    }

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

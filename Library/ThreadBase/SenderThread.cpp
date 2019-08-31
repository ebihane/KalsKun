#include <stdio.h>
#include <stdlib.h>
#include "SenderThread.h"

SenderThread::SenderThread(AdapterBase* const adapter)
 : ThreadBase()
 , m_Adapter(adapter)
{
}

SenderThread::~SenderThread()
{
    finalize();
}

ResultEnum SenderThread::initializeCore()
{
    /* �p�����Ȃ��ꍇ�͉������������Ԃ� */
    return ResultEnum::NormalEnd;
}

void SenderThread::doReconnectInitialize(const bool isFirst)
{
    /* �p�����Ȃ��ꍇ�͉������Ȃ� */
}

ResultEnum SenderThread::doConnectedProc()
{
    /* �p�����Ȃ��ꍇ�͉������������Ԃ� */
    return ResultEnum::NormalEnd;
}

ResultEnum SenderThread::finalizeCore()
{
    /* �p�����Ȃ��ꍇ�͉������������Ԃ� */
    return ResultEnum::NormalEnd;
}

ResultEnum SenderThread::initialize()
{
    ResultEnum  retVal = ResultEnum::AbnormalEnd;
    
    if (m_Adapter == NULL)
    {
        m_Logger->LOG_ERROR("[initialize] m_Adapter allocation failed.\n");
        goto FINISH;
    }

    if (m_Adapter->Open() != ResultEnum::NormalEnd)
    {
        char logStr[80] = { 0 };
        snprintf(&logStr[0], sizeof(logStr), "[initialize] Adapter Open failed. errno[%d]\n", m_Adapter->GetLastError());
        m_Logger->LOG_ERROR(logStr);
        goto FINISH;
    }

    retVal = initializeCore();


FINISH :
    return retVal;
}

ResultEnum SenderThread::doProcedure()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    ResultEnum result = ResultEnum::AbnormalEnd;
    bool isFirst = true;
    bool sendRequest = false;
    EventInfo ev = { 0 };

RECONNECT :

    /* �ڑ����̏��������� */
    doReconnectInitialize(isFirst);
    isFirst = false;

    /* ��������ؒf���� */
    m_Adapter->Close();

    /* �ڑ� */
    result = m_Adapter->Connection();
    if (result != ResultEnum::NormalEnd)
    {
        char logStr[80] = { 0 };
        snprintf(&logStr[0], sizeof(logStr), "[doProcedure] Connection failed. errno[%d]\n", m_Adapter->GetLastError());
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

    /* �ڑ��m�������� */
    result = doConnectedProc();
    if (result != ResultEnum::NormalEnd)
    {
        char logStr[80] = { 0 };
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
        sendRequest = createSendData(&ev);
        if (sendRequest == false)
        {
            if (isStopRequest() == true)
            {
                m_Logger->LOG_INFO("[doProcedure] Thread stop request.\n");
                break;
            }

            delay(100);
            continue;
        }

        result = m_Adapter->Send(&ev, sizeof(EventInfo));
        if (result != ResultEnum::NormalEnd)
        {
            char logStr[80] = { 0 };
            snprintf(&logStr[0], sizeof(logStr), "[doProcedure] Send failed. errno[%d]\n", m_Adapter->GetLastError());
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

ResultEnum SenderThread::finalize()
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

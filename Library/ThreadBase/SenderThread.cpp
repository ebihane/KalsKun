#include <stdio.h>
#include <stdlib.h>
#include "SenderThread.h"

SenderThread::SenderThread(char* const taskName, AdapterBase* const adapter)
 : ThreadBase(taskName)
 , m_SendData(NULL)
 , m_Adapter(adapter)
{
}

SenderThread::~SenderThread()
{
    finalize();
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
    ResultEnum  result = ResultEnum::AbnormalEnd;
    char logStr[80] = { 0 };

    if (m_Adapter == NULL)
    {
        m_Logger->LOG_ERROR("[initialize] m_Adapter allocation failed.\n");
        goto FINISH;
    }

    if (m_Adapter->Open() != ResultEnum::NormalEnd)
    {
        snprintf(&logStr[0], sizeof(logStr), "[initialize] Adapter Open failed. errno[%d]\n", m_Adapter->GetLastError());
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

    if (m_SendData == NULL)
    {
        m_Logger->LOG_ERROR("[initialize] m_SendData is NULL.\n");
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

ResultEnum SenderThread::doProcedure()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    ResultEnum result = ResultEnum::AbnormalEnd;
    bool isFirst = true;
    bool sendRequest = false;
    unsigned long size = 0;
    char logStr[80] = { 0 };

RECONNECT :

    /* �ڑ����̏��������� */
    doReconnectInitialize(isFirst);
    isFirst = false;

    /* ��������ؒf���� */
    m_Adapter->Disconnection();

    /* �ڑ� */
    result = m_Adapter->Connection();
    if (result != ResultEnum::NormalEnd)
    {
        snprintf(&logStr[0], sizeof(logStr), "[doProcedure] Connection failed. errno[%d]\n", m_Adapter->GetLastError());
        m_Logger->LOG_ERROR(logStr);

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
            goto FINISH;
        }
    }

    snprintf(&logStr[0], sizeof(logStr), "[doProcedure] %s Connection establish.\n", &m_TaskName[0]);
    m_Logger->LOG_INFO(logStr);

    /* �ڑ��m�������� */
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
        sendRequest = createSendData(m_SendData, &size);
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

        result = m_Adapter->Send(m_SendData, size);
        if (result != ResultEnum::NormalEnd)
        {
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

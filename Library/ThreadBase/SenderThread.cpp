#include <stdio.h>
#include <stdlib.h>
#include "SenderThread.h"

SenderThread::SenderThread(char* const taskName, AdapterBase* const adapter, const bool responseExist)
 : ThreadBase(taskName)
 , m_SendData(NULL)
 , m_RecvData(NULL)
 , m_RecvSize(0)
 , RESPONSE_EXIST(responseExist)
 , m_Adapter(adapter)
{
}

SenderThread::~SenderThread()
{
    finalize();
}

void SenderThread::doReconnectInitialize(const bool isFirst)
{
    /* Œp³‚µ‚È‚¢ê‡‚Í‰½‚à‚µ‚È‚¢ */
}

ResultEnum SenderThread::doConnectedProc()
{
    /* Œp³‚µ‚È‚¢ê‡‚Í‰½‚à‚¹‚¸³í‚ð•Ô‚· */
    return ResultEnum::NormalEnd;
}

void SenderThread::analyzeResponse(char* const data, const unsigned long size)
{
    /* Œp³‚µ‚È‚¢ê‡‚Í‰½‚à‚µ‚È‚¢ */
}

ResultEnum SenderThread::finalizeCore()
{
    /* Œp³‚µ‚È‚¢ê‡‚Í‰½‚à‚¹‚¸³í‚ð•Ô‚· */
    return ResultEnum::NormalEnd;
}

ResultEnum SenderThread::initialize()
{
    ResultEnum  retVal = ResultEnum::AbnormalEnd;
    ResultEnum  result = ResultEnum::AbnormalEnd;

    if (m_Adapter == NULL)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::initialize] m_Adapter allocation failed.\n", m_TaskName);
        m_Logger->LOG_ERROR(m_LogStr);
        goto FINISH;
    }

    if (m_Adapter->Open() != ResultEnum::NormalEnd)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::initialize] Adapter Open failed. errno[%d]\n", m_TaskName, m_Adapter->GetLastError());
        m_Logger->LOG_ERROR(m_LogStr);
        goto FINISH;
    }

    result = initializeCore();
    if (result != ResultEnum::NormalEnd)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::initialize] initializeCore failed. result[%d]\n", m_TaskName, result);
        m_Logger->LOG_ERROR(m_LogStr);
        goto FINISH;
    }

    if (m_SendData == NULL)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::initialize] m_SendData is NULL.\n", m_TaskName);
        m_Logger->LOG_ERROR(m_LogStr);
        goto FINISH;
    }

    if (RESPONSE_EXIST == true)
    {
        if (m_RecvData == NULL)
        {
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::initialize] m_RecvData is NULL.\n", m_TaskName);
            m_Logger->LOG_ERROR(m_LogStr);
            goto FINISH;
        }
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

RECONNECT :

    snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] Connection start.\n", m_TaskName);
    m_Logger->LOG_INFO(m_LogStr);

    /* Ú‘±Žž‚Ì‰Šú‰»ˆ— */
    doReconnectInitialize(isFirst);
    isFirst = false;

    /* ‚¢‚Á‚½‚ñØ’f‚·‚é */
    m_Adapter->Disconnection();

    /* Ú‘± */
    result = m_Adapter->Connection();
    if (result != ResultEnum::NormalEnd)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] Connection failed. errno[%d]\n", m_TaskName, m_Adapter->GetLastError());
        m_Logger->LOG_ERROR(m_LogStr);

        if (result == ResultEnum::Reconnect)
        {
            if (isStopRequest() == true)
            {
                snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] Thread Stop request.\n", m_TaskName);
                m_Logger->LOG_INFO(m_LogStr);
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
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] Thread finish.\n", m_TaskName);
            m_Logger->LOG_ERROR(m_LogStr);
            goto FINISH;
        }
    }

    snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] Connection establish.\n", m_TaskName);
    m_Logger->LOG_INFO(m_LogStr);

    /* Ú‘±Šm—§Žžˆ— */
    result = doConnectedProc();
    if (result != ResultEnum::NormalEnd)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] doConnectedProc failed.\n", m_TaskName);
        m_Logger->LOG_ERROR(m_LogStr);

        if (result == ResultEnum::Reconnect)
        {
            delay(3000);
            goto RECONNECT;
        }
        else
        {
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] Thread finish.\n", m_TaskName);
            m_Logger->LOG_ERROR(m_LogStr);
            goto FINISH;
        }
    }

    snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] Main loop enter.\n", m_TaskName);
    m_Logger->LOG_INFO(m_LogStr);

    while (1)
    {
        sendRequest = createSendData(m_SendData, &size);
        if (sendRequest == false)
        {
            if (isStopRequest() == true)
            {
                snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] Thread stop request.\n", m_TaskName);
                m_Logger->LOG_INFO(m_LogStr);
                break;
            }

            delay(100);
            continue;
        }

        result = m_Adapter->Send(m_SendData, size);
        if (result != ResultEnum::NormalEnd)
        {
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] Send failed. errno[%d]\n", m_TaskName, m_Adapter->GetLastError());
            m_Logger->LOG_ERROR(m_LogStr);

            if (result == ResultEnum::Reconnect)
            {
                goto RECONNECT;
            }
            else
            {
                snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] Thread finish.\n", m_TaskName);
                m_Logger->LOG_ERROR(m_LogStr);
                goto FINISH;
            }
        }

        if (RESPONSE_EXIST == true)
        {
            result = m_Adapter->Receive(m_RecvData, m_RecvSize);
            if (result != ResultEnum::NormalEnd)
            {
                snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] Receive failed. errno[%d]\n", m_TaskName, m_Adapter->GetLastError());
                m_Logger->LOG_ERROR(m_LogStr);

                if (result == ResultEnum::Reconnect)
                {
                    goto RECONNECT;
                }
                else
                {
                    snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] Thread finish.\n", m_TaskName);
                    m_Logger->LOG_ERROR(m_LogStr);
                    goto FINISH;
                }
            }

            analyzeResponse(&m_RecvData[0], m_RecvSize);
        }
    }

    snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] Main loop exit.\n", m_TaskName);
    m_Logger->LOG_INFO(m_LogStr);

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

#include <stdio.h>
#include <stdlib.h>
#include "ReceiverThread.h"

ReceiverThread::ReceiverThread(char* const taskName, AdapterBase* const adapter, const bool responseExist)
 : ThreadBase(taskName)
 , m_RecvData(NULL)
 , RESPONSE_EXIST(responseExist)
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
    /* Œp³‚µ‚È‚¢ê‡‚Í‰½‚à‚µ‚È‚¢ */
}

ResultEnum ReceiverThread::doConnectedProc()
{
    /* Œp³‚µ‚È‚¢ê‡‚Í‰½‚à‚¹‚¸³í‚ð•Ô‚· */
    return ResultEnum::NormalEnd;
}

unsigned long ReceiverThread::createResponse(char* const buffer)
{
    /* Œp³‚µ‚È‚¢ê‡‚ÍƒTƒCƒY 0 ‚ð•Ô‚· */
    return 0;
}

ResultEnum ReceiverThread::finalizeCore()
{
    /* Œp³‚µ‚È‚¢ê‡‚Í‰½‚à‚¹‚¸³í‚ð•Ô‚· */
    return ResultEnum::NormalEnd;
}

ResultEnum ReceiverThread::initialize()
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
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::initialize] m_Adapter Open failed. errno[%d]\n", m_TaskName, m_Adapter->GetLastError());
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

    if (m_RecvData == NULL)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::initialize] m_SendData is NULL.\n", m_TaskName);
        m_Logger->LOG_ERROR(m_LogStr);
        goto FINISH;
    }

    if (RESPONSE_EXIST == true)
    {
        if (m_ResponseData == NULL)
        {
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::initialize] m_ResponseData is NULL.\n", m_TaskName);
            m_Logger->LOG_ERROR(m_LogStr);
            goto FINISH;
        }
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
    unsigned long responseSize = 0;


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
        result = m_Adapter->IsReceivable(receivable);
        if (result != ResultEnum::NormalEnd)
        {
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] IsReceivable failed. errno[%d]\n", m_TaskName, m_Adapter->GetLastError());
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

        if (receivable == false)
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

        result = receive();
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

        /* ŽóMƒf[ƒ^‚Ì‰ðÍ */
        result = analyze(&m_RecvData[0]);
        if (result != ResultEnum::NormalEnd)
        {
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] analyze failed.\n", m_TaskName);
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
            responseSize = createResponse(&m_ResponseData[0]);
            result = m_Adapter->Send(&m_ResponseData[0], responseSize);
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
        }
    }

    snprintf(&m_LogStr[0], sizeof(m_LogStr), "[%s::doProcedure] Main loop exit.\n", m_TaskName);
    m_Logger->LOG_INFO(m_LogStr);

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

#include <stdio.h>
#include <stdlib.h>
#include "SenderThread.h"

SenderThread::SenderThread(char* const ipAddress, const unsigned short portNo)
 : ThreadBase()
 , m_TcpClient(NULL)
 , m_Queue(NULL)
{
    m_Queue = new Queue((char*)"SenderThread");
    m_TcpClient = new TcpClient(ipAddress, portNo);
}

SenderThread::~SenderThread()
{
    /* nop. */
}

ResultEnum SenderThread::initialize()
{
    ResultEnum  retVal = ResultEnum::AbnormalEnd;
    
    if (m_TcpClient->Open() != ResultEnum::NormalEnd)
    {
        char logStr[80] = { 0 };
        snprintf(&logStr[0], sizeof(logStr), "[initialize] TcpClient Open failed. errno[%d]\n", m_TcpClient->GetLastError());
        m_Logger->LOG_ERROR(logStr);
        goto FINISH;
    }

    if (m_Queue->Open() != ResultEnum::NormalEnd)
    {
        char logStr[80] = { 0 };
        snprintf(&logStr[0], sizeof(logStr), "[initialize] Queue opne failed. errno[%d]\n", m_Queue->GetLastError());
        m_Logger->LOG_ERROR(logStr);
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
    bool receivable = false;
    EventInfo ev = { 0 };

RECONNECT :

    m_TcpClient->Close();

    result = m_TcpClient->Connection();
    if (result != ResultEnum::NormalEnd)
    {
        char logStr[80] = { 0 };
        snprintf(&logStr[0], sizeof(logStr), "[doProcedure] Connection failed. errno[%d]\n", m_TcpClient->GetLastError());
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
        receivable = false;
        if (m_Queue->IsReceivable(receivable) != ResultEnum::NormalEnd)
        {
            char logStr[80] = { 0 };
            snprintf(&logStr[0], sizeof(logStr), "[doProcedure] Queue IsReceivable failed. errno[%d]\n", m_Queue->GetLastError());
            m_Logger->LOG_ERROR(logStr);
            goto FINISH;
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

        if (m_Queue->Receive(&ev) != ResultEnum::NormalEnd)
        {
            char logStr[80] = { 0 };
            snprintf(&logStr[0], sizeof(logStr), "[doProcedure] Queue Receive failed. errno[%d]\n", m_Queue->GetLastError());
            m_Logger->LOG_ERROR(logStr);
            goto FINISH;
        }

        result = m_TcpClient->Send(&ev, sizeof(EventInfo));
        if (result != ResultEnum::NormalEnd)
        {
            char logStr[80] = { 0 };
            snprintf(&logStr[0], sizeof(logStr), "[doProcedure] Send failed. errno[%d]\n", m_TcpClient->GetLastError());
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
    if (m_TcpClient != NULL)
    {
        m_TcpClient->Close();
        delete m_TcpClient;
        m_TcpClient = NULL;
    }

    return ResultEnum::NormalEnd;
}

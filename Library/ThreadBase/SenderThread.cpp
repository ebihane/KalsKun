#include <stdio.h>
#include <stdlib.h>
#include "SenderThread.h"

SenderThread::SenderThread(char* const ipAddress, const unsigned short portNo)
 : ThreadBase()
 , m_TcpClient(NULL)
{
    m_TcpClient = new TcpClient(ipAddress, portNo);
}

SenderThread::~SenderThread()
{
    finalize();
}

ResultEnum SenderThread::initializeCore()
{
    /* Œp³‚µ‚È‚¢ê‡‚Í‰½‚à‚¹‚¸³í‚ð•Ô‚· */
    return ResultEnum::NormalEnd;
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

ResultEnum SenderThread::finalizeCore()
{
    /* Œp³‚µ‚È‚¢ê‡‚Í‰½‚à‚¹‚¸³í‚ð•Ô‚· */
    return ResultEnum::NormalEnd;
}

ResultEnum SenderThread::initialize()
{
    ResultEnum  retVal = ResultEnum::AbnormalEnd;
    
    if (m_TcpClient == NULL)
    {
        m_Logger->LOG_ERROR("[initialize] m_TcpClient allocation failed.\n");
        goto FINISH;
    }

    if (m_TcpClient->Open() != ResultEnum::NormalEnd)
    {
        char logStr[80] = { 0 };
        snprintf(&logStr[0], sizeof(logStr), "[initialize] TcpClient Open failed. errno[%d]\n", m_TcpClient->GetLastError());
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

    /* Ú‘±Žž‚Ì‰Šú‰»ˆ— */
    doReconnectInitialize(isFirst);
    isFirst = false;

    /* ‚¢‚Á‚½‚ñØ’f‚·‚é */
    m_TcpClient->Close();

    /* Ú‘± */
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

    /* Ú‘±Šm—§Žžˆ— */
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
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    if (m_TcpClient != NULL)
    {
        m_TcpClient->Close();
        delete m_TcpClient;
        m_TcpClient = NULL;
    }

    retVal = finalizeCore();

    return retVal;
}

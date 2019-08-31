#include <stdio.h>
#include <stdlib.h>
#include "ReceiverThread.h"

ReceiverThread::ReceiverThread(AdapterBase* const adapter)
 : ThreadBase()
 , m_Adapter(adapter)
{
    /* nop. */
}

ReceiverThread::~ReceiverThread()
{
    finalize();
}

ResultEnum ReceiverThread::initializeCore()
{
    /* Œp³‚µ‚È‚¢ê‡‚Í‰½‚à‚¹‚¸³í‚ð•Ô‚· */
    return ResultEnum::NormalEnd;
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

ResultEnum ReceiverThread::finalizeCore()
{
    /* Œp³‚µ‚È‚¢ê‡‚Í‰½‚à‚¹‚¸³í‚ð•Ô‚· */
    return ResultEnum::NormalEnd;
}

ResultEnum ReceiverThread::initialize()
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
        snprintf(&logStr[0], sizeof(logStr), "[initialize] m_Adapter Open failed. errno[%d]\n", m_Adapter->GetLastError());
        m_Logger->LOG_ERROR(logStr);
        goto FINISH;
    }

    retVal = initializeCore();


FINISH :
    return retVal;
}

ResultEnum ReceiverThread::doProcedure()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    ResultEnum result = ResultEnum::AbnormalEnd;
    bool isFirst = true;
    bool receivable = false;
    EventInfo ev = { 0 };


RECONNECT :

    /* Ú‘±Žž‚Ì‰Šú‰»ˆ— */
    doReconnectInitialize(isFirst);
    isFirst = false;

    /* ‚¢‚Á‚½‚ñØ’f‚·‚é */
    m_Adapter->Disconnection();

    /* Ú‘± */
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
        result = m_Adapter->IsReceivable(receivable);
        if (result != ResultEnum::NormalEnd)
        {
            char logStr[80] = { 0 };
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

        result = m_Adapter->Receive(&ev, sizeof(EventInfo));
        if (result != ResultEnum::NormalEnd)
        {
            char logStr[80] = { 0 };
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

        /* ŽóMƒf[ƒ^‚Ì‰ðÍ */
        result = analyze(&ev);
        if (result != ResultEnum::NormalEnd)
        {
            char logStr[80] = { 0 };
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

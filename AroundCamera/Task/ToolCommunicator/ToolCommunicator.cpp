#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Parts/ShareMemory/ShareMemory.h"
#include "ToolCommunicator.h"

ToolCommunicator::ToolCommunicator()
 : ThreadBase((char *)"ToolComm")
 , m_TcpServer(TOOL_TO_COMMANDER_PORT)
{
    /* nop. */
}

ToolCommunicator::~ToolCommunicator()
{
    /* nop. */
}

ResultEnum ToolCommunicator::initialize()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    ResultEnum result = ResultEnum::AbnormalEnd;

    result = m_TcpServer.Open();
    if (result != ResultEnum::NormalEnd)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[initialize] m_TcpServer Open failed. errno[%d]\n", m_TcpServer.GetLastError());
        m_Logger->LOG_ERROR(m_LogStr);
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

ResultEnum ToolCommunicator::doProcedure()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    ResultEnum result = ResultEnum::AbnormalEnd;
    bool receivable = false;
    EventInfo sEv = { 0 };

RECONNECT:

    /* ‚¢‚Á‚½‚ñØ’f‚·‚é */
    m_TcpServer.Disconnection();

    /* Ú‘± */
    result = m_TcpServer.Connection();
    if (result != ResultEnum::NormalEnd)
    {
        if (result == ResultEnum::Reconnect)
        {
            if (isStopRequest() == true)
            {
                m_Logger->LOG_INFO("[doProcedure] Thread Stop request.\n");
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
            m_Logger->LOG_ERROR("[doProcedure] Thread finish.\n");
            goto FINISH;
        }
    }

    m_Logger->LOG_INFO("[doProcedure] Connection establish.\n");
    while (1)
    {
        result = m_TcpServer.IsReceivable(receivable);
        if (result != ResultEnum::NormalEnd)
        {
            if (isStopRequest() == true)
            {
                m_Logger->LOG_INFO("[doProcedure] Thread Stop request.\n");
                break;
            }
            else if (result == ResultEnum::Reconnect)
            {
                m_Logger->LOG_INFO("[doProcedure] Disconnect.\n");
                goto RECONNECT;
            }
            else
            {
                snprintf(&m_LogStr[0], sizeof(m_LogStr), "[doProcedure] m_TcpServer IsReceivable failed. errno[%d]\n", m_TcpServer.GetLastError());
                m_Logger->LOG_ERROR(m_LogStr);
                goto FINISH;
            }
        }

        result = m_TcpServer.Receive(&sEv, sizeof(sEv));
        if (result != ResultEnum::NormalEnd)
        {
            if (result == ResultEnum::Reconnect)
            {
                m_Logger->LOG_INFO("[doProcedure] Disconnect.\n");
                goto RECONNECT;
            }
            else
            {
                snprintf(&m_LogStr[0], sizeof(m_LogStr), "[doProcedure] m_TcpServer Receive failed. errno[%d]\n", m_TcpServer.GetLastError());
                m_Logger->LOG_ERROR(m_LogStr);
                goto FINISH;
            }
        }

        result = analyzeAndExecuteEvent(&sEv);
        if (result != ResultEnum::NormalEnd)
        {
            if (result == ResultEnum::Reconnect)
            {
                m_Logger->LOG_INFO("[doProcedure] Disconnect.\n");
                goto RECONNECT;
            }
            else
            {
                snprintf(&m_LogStr[0], sizeof(m_LogStr), "[doProcedure] analyzeAndExecuteEvent failed. errno[%d]\n", m_TcpServer.GetLastError());
                m_Logger->LOG_ERROR(m_LogStr);
                goto FINISH;
            }
        }
    }

    m_Logger->LOG_INFO("[doProcedure] Main loop exit.\n");

    retVal = ResultEnum::NormalEnd;

FINISH:
    return retVal;
}

ResultEnum ToolCommunicator::finalize()
{
    m_TcpServer.Close();
    return ResultEnum::NormalEnd;
}

ResultEnum ToolCommunicator::analyzeAndExecuteEvent(EventInfo* const pEv)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    switch (pEv->Code)
    {
        case 3001 :
            retVal = doDateTimeAdjust(pEv);
            break;

        case 9999 :
            retVal = doDisconnectEvent();
            break;

        default :
            break;
    }

    return retVal;
}

ResultEnum ToolCommunicator::doDateTimeAdjust(EventInfo* const pEv)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    long year = pEv->lParam[0] / 10000;
    long month = (pEv->lParam[0] % 10000) / 100;
    long day = pEv->lParam[0] % 100;
    long hour = pEv->lParam[1] / 10000;
    long minute = (pEv->lParam[1] % 10000) / 100;
    long second = pEv->lParam[1] % 100;

    snprintf(&m_LogStr[0], sizeof(m_LogStr), "Time Adjust : %04d/%02d/%02d %02d:%02d:%02d\n", year, month, day, hour, minute, second);
    m_Logger->LOG_INFO(m_LogStr);

    char command[120] = { 0 };
    snprintf(&command[0], sizeof(command), "sudo date --set '%04d/%02d/%02d %02d:%02d:%02d'", year, month, day, hour, minute, second);
    int result = system(&command[0]);
    
    long retCode = result;
    retVal = m_TcpServer.Send(&retCode, sizeof(retCode));

    return retVal;
}

ResultEnum ToolCommunicator::doDisconnectEvent()
{
    long result = ResultEnum::NormalEnd;
    m_TcpServer.Send(&result, sizeof(result));

    return ResultEnum::Reconnect;
}

#include <stdio.h>
#include "Parts/ShareMemory/ShareMemory.h"
#include "AroundCameraComm.h"

AroundCameraComm::AroundCameraComm()
 : ThreadBase()
 , m_TcpServer(10001)
{
    /* nop. */
}

AroundCameraComm::~AroundCameraComm()
{
    m_TcpServer.Close();
}

ResultEnum AroundCameraComm::initialize()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    if (m_TcpServer.Open() != ResultEnum::NormalEnd)
    {
        char logStr[80] = { 0 };
        snprintf(&logStr[0], sizeof(logStr), "[initialize] Socket open failed. errno[%d]\n", m_TcpServer.GetLastError());
        m_Logger->LOG_ERROR(logStr);
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

ResultEnum AroundCameraComm::doProcedure()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    ResultEnum result = ResultEnum::AbnormalEnd;
    bool receivable = false;
    EventInfo ev = { 0 };

RECONNECT :

    m_TcpServer.Disconnection();

    result = m_TcpServer.Connection();
    if (result != ResultEnum::NormalEnd)
    {
        char logStr[80] = { 0 };
        snprintf(&logStr[0], sizeof(logStr), "[doProcedure] Socket connect failed. errno[%d]\n", m_TcpServer.GetLastError());
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

    m_Logger->LOG_INFO("[doProcedure] Connection establish.\n");
    delay(1000);

    m_Logger->LOG_INFO("[doProcedure] Main loop start.\n");
    while (1)
    {
        result = m_TcpServer.IsReceivable(receivable);
        if (result != ResultEnum::NormalEnd)
        {
            char logStr[80] = { 0 };
            snprintf(&logStr[0], sizeof(logStr), "[doProcedure] Socket Receive Check failed. errno[%d]\n", m_TcpServer.GetLastError());
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
                break;
            }
            
            delay(100);
            continue;
        }

        result = m_TcpServer.Receive(&ev, sizeof(EventInfo));
        if (result != ResultEnum::NormalEnd)
        {
            char logStr[80] = { 0 };
            snprintf(&logStr[0], sizeof(logStr), "[doProcedure] Socket Receive failed. errno[%d]\n", m_TcpServer.GetLastError());
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

        pShareMemory->AroundCamera.ReceiveCount = ev.lParam[0];
        pShareMemory->AroundCamera.SystemError = ev.lParam[1];
        pShareMemory->AroundCamera.PersonDetect = ev.lParam[2];
    }
    m_Logger->LOG_INFO("[doProcedure] Main loop finish.\n");

    retVal = ResultEnum::NormalEnd;

FINISH:
    return retVal;
}

ResultEnum AroundCameraComm::finalize()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    m_TcpServer.Close();

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}
#include "Parts/ShareMemory/ShareMemory.h"
#include "StateSender.h"

StateSender::StateSender()
 : ThreadBase()
 , m_TcpClient((char *)"192.168.3.1", 10001)
{
    /* nop. */
}

StateSender::~StateSender()
{
    m_TcpClient.Close();
}

ResultEnum StateSender::initialize()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;


    if (m_TcpClient.Open() != ResultEnum::NormalEnd)
    {
        char logStr[80] = { 0 };
        snprintf(&logStr[0], sizeof(logStr), "[initialize] Socket open failed. errno[%d]\n", m_TcpClient.GetLastError());
        m_Logger->LOG_ERROR(logStr);
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

ResultEnum StateSender::doProcedure()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    ResultEnum result = ResultEnum::AbnormalEnd;
    EventInfo ev = { 0 };
    long sendCount = 0;
    
RECONNECT :

    m_TcpClient.Disconnection();

    result = m_TcpClient.Connection();
    if (result != ResultEnum::NormalEnd)
    {
        char logStr[80] = { 0 };
        snprintf(&logStr[0], sizeof(logStr), "[doProcedure] Socket connect failed. errno[%d]\n", m_TcpClient.GetLastError());
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
    delay(5000);

    m_Logger->LOG_INFO("[doProcedure] Main loop start.\n");
    while (1)
    {
        if (isStopRequest() == true)
        {
            break;
        }

        /* ‘—M‚·‚éî•ñ‚ð¶¬ */
        ev.Code = 1;
        ev.Result = ResultEnum::NormalEnd;
        ev.lParam[0] = sendCount;
        ev.lParam[1] = pShareMemory->SystemError;
        ev.lParam[2] = (long)pShareMemory->PatrolState;

        result = m_TcpClient.Send(&ev, sizeof(EventInfo));
        if (result != ResultEnum::NormalEnd)
        {
            char logStr[80] = { 0 };
            snprintf(&logStr[0], sizeof(logStr), "[doProcedure] Socket send failed. errno[%d]\n", m_TcpClient.GetLastError());
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

        delay(100);
    }

    m_Logger->LOG_INFO("[doProcedure] Main loop finish.\n");
    
    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

ResultEnum StateSender::finalize()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    m_TcpClient.Close();

    retVal = ResultEnum::NormalEnd;
    return retVal;
}

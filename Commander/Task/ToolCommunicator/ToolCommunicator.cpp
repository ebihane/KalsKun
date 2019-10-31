#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Parts/ShareMemory/ShareMemory.h"
#include "ToolCommunicator.h"

ToolCommunicator::ToolCommunicator()
 : ThreadBase((char *)"ToolComm")
 , m_TcpServer(TOOL_TO_COMMANDER_PORT)
 , m_Setting(NULL)
 , m_AreaMap(NULL)
 , m_MoveMap(NULL)
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

    /* 各インスタンス取得 */
    m_Setting = SettingManager::GetInstance();
    m_AreaMap = AreaMap::GetInstance();
    m_MoveMap = MoveMap::GetInstance();

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

    /* いったん切断する */
    m_TcpServer.Disconnection();

    /* 接続 */
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
        case 1000 :
            retVal = doSettingRead();
            break;

        case 1001 :
            retVal = doSettingChange(pEv);
            break;

        case 2001 :
            retVal = doStateMonitor();
            break;

        case 2002 :
            retVal = doAreaMapMonitor();
            break;

        case 2003 :
            retVal = doMoveMapMonitor();
            break;

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

ResultEnum ToolCommunicator::doSettingRead()
{
    m_Logger->LOG_INFO("[doSettingRead]::Enter.\n");

    ResultEnum retVal = ResultEnum::AbnormalEnd;
    unsigned long index = 0;
    unsigned long sendLength = 0;
    SizeStr robotSize;
    SizeStr farmSize;
    RectStr mapCount;
    SettingManager::TimeSettingStr kusakariStart;
    SettingManager::TimeSettingStr yakeiStart;
    float moveEndRate = 0.0f;
    char* sendBuffer = NULL;

    sendLength += sizeof(robotSize);
    sendLength += sizeof(farmSize);
    sendLength += sizeof(mapCount);
    sendLength += sizeof(kusakariStart);
    sendLength += sizeof(yakeiStart);
    sendLength += sizeof(moveEndRate);

    sendBuffer = new char[sendLength];
    if (sendBuffer == NULL)
    {
        m_Logger->LOG_ERROR("[doSettingRead] sendBuffer allocation failed.\n");
        goto FINISH;
    }

    memset(&sendBuffer[0], 0x00, sendLength);

    m_Setting->GetRobotSize(&robotSize);
    memcpy(&sendBuffer[index], &robotSize, sizeof(robotSize));
    index += sizeof(robotSize);

    m_Setting->GetFarmSize(&farmSize);
    memcpy(&sendBuffer[index], &farmSize, sizeof(farmSize));
    index += sizeof(farmSize);

    m_Setting->GetMapCount(&mapCount);
    memcpy(&sendBuffer[index], &mapCount, sizeof(mapCount));
    index += sizeof(mapCount);

    kusakariStart = m_Setting->GetKusakariStartTime();
    memcpy(&sendBuffer[index], &kusakariStart, sizeof(kusakariStart));
    index += sizeof(kusakariStart);

    yakeiStart = m_Setting->GetYakeiStartTime();
    memcpy(&sendBuffer[index], &yakeiStart, sizeof(yakeiStart));
    index += sizeof(yakeiStart);

    moveEndRate = m_Setting->GetMoveEndRate();
    memcpy(&sendBuffer[index], & moveEndRate, sizeof(moveEndRate));
    index += sizeof(moveEndRate);

    retVal = m_TcpServer.Send(&sendBuffer[0], sendLength);

FINISH :

    if (sendBuffer != NULL)
    {
        delete[] sendBuffer;
        sendBuffer = NULL;
    }

    m_Logger->LOG_INFO("[doSettingRead]::Exit.\n");
    return retVal;
}

ResultEnum ToolCommunicator::doSettingChange(EventInfo* const pEv)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    long response = -1;

    switch (pEv->lParam[0])
    {
        case 0 :
            setRobotSizeSetting(pEv->fParam[0], pEv->fParam[1]);
            response = 0;
            break;

        case 1 :
            setFarmSizeSetting(pEv->fParam[0], pEv->fParam[1]);
            response = 0;
            break;

        case 2 :
            {
                SettingManager::TimeSettingStr kusakariDate;
                kusakariDate.DayOfWeek = (SettingManager::DayOfWeekEnum)pEv->lParam[1];
                kusakariDate.Hour = (unsigned short)pEv->lParam[2];
                kusakariDate.Minute = (unsigned short)pEv->lParam[3];
                m_Setting->SetKusakariStartTime(&kusakariDate);
                snprintf(&m_LogStr[0], sizeof(m_LogStr), "[KusakariStartChange] Change. [%d %02d:%02d]\n", kusakariDate.DayOfWeek, kusakariDate.Hour, kusakariDate.Minute);
                m_Logger->LOG_INFO(m_LogStr);
            }
            response = 0;
            break;

        case 3:
            {
                SettingManager::TimeSettingStr yakeiDate;
                yakeiDate.DayOfWeek = (SettingManager::DayOfWeekEnum)pEv->lParam[1];
                yakeiDate.Hour = (unsigned short)pEv->lParam[2];
                yakeiDate.Minute = (unsigned short)pEv->lParam[3];
                m_Setting->SetYakeStartTime(&yakeiDate);
                snprintf(&m_LogStr[0], sizeof(m_LogStr), "[YakeiStartChange] Change. [%d %02d:%02d]\n", yakeiDate.DayOfWeek, yakeiDate.Hour, yakeiDate.Minute);
                m_Logger->LOG_INFO(m_LogStr);
            }
            response = 0;
            break;

        case 4:
            m_Setting->SetMoveEndRate(pEv->fParam[0]);
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[MoveEndRateChange] Change. [%.1f]\n", pEv->fParam[0]);
            m_Logger->LOG_INFO(m_LogStr);
            break;

        default :
            break;
    }

    if (response == 0)
    {
        m_Setting->Save();
    }

    retVal = m_TcpServer.Send(&response, sizeof(response));

    return retVal;
}

ResultEnum ToolCommunicator::doStateMonitor()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    unsigned long size = 0;
    
    size += sizeof(pShareMemory->Commander);
    size += sizeof(pShareMemory->Motor);
    size += sizeof(pShareMemory->FrontCamera);
    size += sizeof(pShareMemory->AnimalCamera);
    size += sizeof(pShareMemory->AroundCamera);

    char* sendBuffer = new char[size];
    if (sendBuffer == NULL)
    {
        m_Logger->LOG_ERROR("[doStateMonitor] sendBuffer allocation failed.\n");
        goto FINISH;
    }

    memcpy(&sendBuffer[0], pShareMemory, size);

    retVal = m_TcpServer.Send(&sendBuffer[0], size);

FINISH :

    if (sendBuffer != NULL)
    {
        delete[] sendBuffer;
        sendBuffer = NULL;
    }

    return retVal;
}

ResultEnum ToolCommunicator::doAreaMapMonitor()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    SettingManager* setting = SettingManager::GetInstance();
    RectStr mapCount = { 0 };
    setting->GetMapCount(&mapCount);

    char* sendBuffer = new char[mapCount.X * mapCount.Y];
    if (sendBuffer == NULL)
    {
        m_Logger->LOG_ERROR("[doAreaMapMonitor] sendBuffer allocation failed.\n");
        goto FINISH;
    }

    m_AreaMap->Get(&sendBuffer[0]);

    retVal = m_TcpServer.Send(&sendBuffer[0], mapCount.X * mapCount.Y);

FINISH :

    if (sendBuffer != NULL)
    {
        delete[] sendBuffer;
        sendBuffer = NULL;
    }

    return retVal;
}

ResultEnum ToolCommunicator::doMoveMapMonitor()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    SettingManager* setting = SettingManager::GetInstance();
    RectStr mapCount = { 0 };
    setting->GetMapCount(&mapCount);

    char* sendBuffer = new char[mapCount.X * mapCount.Y];
    if (sendBuffer == NULL)
    {
        m_Logger->LOG_ERROR("[doMoveMapMonitor] sendBuffer allocation failed.\n");
        goto FINISH;
    }

    m_MoveMap->Get(&sendBuffer[0]);

    retVal = m_TcpServer.Send(&sendBuffer[0], mapCount.X * mapCount.Y);

FINISH:

    if (sendBuffer != NULL)
    {
        delete[] sendBuffer;
        sendBuffer = NULL;
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

void ToolCommunicator::setRobotSizeSetting(const float vertical, const float horizontal)
{
    snprintf(&m_LogStr[0], sizeof(m_LogStr), "[setRobotSizeSetting] V[%f] H[%f]\n", vertical, horizontal);
    m_Logger->LOG_INFO(m_LogStr);

    m_AreaMap->Free();
    m_MoveMap->Free();

    SizeStr robotSize;
    robotSize.Vertical = vertical;
    robotSize.Horizontal = horizontal;
    m_Setting->SetRobotSize(&robotSize);
    m_Setting->UpdateMapCount();

    m_AreaMap->Allocate();
    m_MoveMap->Allocate();

    m_AreaMap->SetInitialData();
    m_MoveMap->SetInitialData();


    m_AreaMap->Save();
    m_MoveMap->Save();
}

void ToolCommunicator::setFarmSizeSetting(const float vertical, const float horizontal)
{
    snprintf(&m_LogStr[0], sizeof(m_LogStr), "[setFarmSizeSetting] V[%f] H[%f]\n", vertical, horizontal);
    m_Logger->LOG_INFO(m_LogStr);

    m_AreaMap->Free();
    m_MoveMap->Free();

    SizeStr farmSize;
    farmSize.Vertical = vertical;
    farmSize.Horizontal = horizontal;
    m_Setting->SetFarmSize(&farmSize);
    m_Setting->UpdateMapCount();

    m_AreaMap->Allocate();
    m_MoveMap->Allocate();

    m_AreaMap->SetInitialData();
    m_MoveMap->SetInitialData();


    m_AreaMap->Save();
    m_MoveMap->Save();
}

#pragma once

#include "Socket/TcpServer/TcpServer.h"
#include "Parts/Setting/SettingManager.h"
#include "Parts/MappingData/AreaMap.h"
#include "Parts/MappingData/MoveMap.h"
#include "ThreadBase/ThreadBase.h"

class ToolCommunicator : public ThreadBase
{
public:

    ToolCommunicator();
    virtual ~ToolCommunicator();

protected:


private:

    TcpServer  m_TcpServer;
    SettingManager* m_Setting;
    AreaMap* m_AreaMap;
    MoveMap* m_MoveMap;

    ResultEnum initialize();
    ResultEnum doProcedure();
    ResultEnum finalize();

    ResultEnum analyzeAndExecuteEvent(EventInfo* const pEv);
    ResultEnum doSettingRead();
    ResultEnum doSettingChange(EventInfo* const pEv);
    ResultEnum doStateMonitor();
    ResultEnum doAreaMapMonitor();
    ResultEnum doMoveMapMonitor();
    ResultEnum doDateTimeAdjust(EventInfo* const pEv);
    ResultEnum doDisconnectEvent();

    void setRobotSizeSetting(const float length, const float width);
    void setFarmSizeSetting(const float length, const float width);

};

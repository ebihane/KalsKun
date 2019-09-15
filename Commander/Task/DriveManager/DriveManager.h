#pragma once

#include "Include/Common.h"
#include "ThreadBase/LoopThreadBase.h"

class DriveManager : public LoopThreadBase
{
public :

    DriveManager();
    virtual ~DriveManager();

protected :


private :

    ResultEnum doMainProc();

};

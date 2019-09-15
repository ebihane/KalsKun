#include "DriveManager.h"

DriveManager::DriveManager()
 : LoopThreadBase(100)
{

}

DriveManager::~DriveManager()
{
    /* nop. */
}

ResultEnum DriveManager::doMainProc()
{
    return ResultEnum::NormalEnd;
}
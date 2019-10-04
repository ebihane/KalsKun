#pragma once

#include "Parts/PositionConverter/ConverterBase.h"

class GyroConverter : public ConverterBase
{
public :

    GyroConverter();
    virtual ~GyroConverter();

    RectStr Convert(const double baseX, const double baseY);

protected :

private :

    SettingManager* m_SettingManager;

};

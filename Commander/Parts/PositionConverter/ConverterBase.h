#pragma once

#include "Include/Common.h"

class ConverterBase
{
public :

    ConverterBase() {};
    virtual ~ConverterBase() {};

    virtual RectStr Convert(const double baseX, const double baseY) = 0;

};

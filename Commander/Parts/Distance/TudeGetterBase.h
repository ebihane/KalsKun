#pragma once

#include "Include/Common.h"
#include "Parts/ShareMemory/ShareMemory.h"

class TudeGetterBase
{
public :

    TudeGetterBase() {};
    virtual ~TudeGetterBase() {};

    virtual ResultEnum Get(TudeStr* const tude) = 0;

protected :

private :

};

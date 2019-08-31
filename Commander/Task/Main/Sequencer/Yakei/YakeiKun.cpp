#include "YakeiKun.h"

YakeiKun::YakeiKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_YAKEI)
{

}

YakeiKun::~YakeiKun()
{

}

void YakeiKun::Initialize()
{

}

void YakeiKun::Destroy()
{

}

SequencerBase::SequenceTypeEnum YakeiKun::Process()
{
    return SequencerBase::SequenceTypeEnum::E_SEQ_YAKEI;
}

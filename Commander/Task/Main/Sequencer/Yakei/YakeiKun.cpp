#include "YakeiKun.h"

YakeiKun::YakeiKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_YAKEI)
{

}

YakeiKun::~YakeiKun()
{

}

ResultEnum YakeiKun::initializeCore()
{
    return ResultEnum::NormalEnd;
}

void YakeiKun::destroyCore()
{

}

SequencerBase::SequenceTypeEnum YakeiKun::processCore()
{
    return SequencerBase::SequenceTypeEnum::E_SEQ_YAKEI;
}

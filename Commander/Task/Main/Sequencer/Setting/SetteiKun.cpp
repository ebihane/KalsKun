#include "SetteiKun.h"

SetteiKun::SetteiKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_SETTING)
{

}

SetteiKun::~SetteiKun()
{

}

void SetteiKun::initializeCore()
{

}

void SetteiKun::destroyCore()
{

}

SequencerBase::SequenceTypeEnum SetteiKun::processCore()
{
    return SequencerBase::SequenceTypeEnum::E_SEQ_SETTING;
}

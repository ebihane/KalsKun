#include "SetteiKun.h"

SetteiKun::SetteiKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_SETTING)
{

}

SetteiKun::~SetteiKun()
{

}

void SetteiKun::Initialize()
{

}

void SetteiKun::Destroy()
{

}

SequencerBase::SequenceTypeEnum SetteiKun::Process()
{
    return SequencerBase::SequenceTypeEnum::E_SEQ_SETTING;
}

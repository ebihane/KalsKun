#include "ErrorKun.h"

ErrorKun::ErrorKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_ERROR)
{

}

ErrorKun::~ErrorKun()
{

}

void ErrorKun::Initialize()
{

}

void ErrorKun::Destroy()
{

}

SequencerBase::SequenceTypeEnum ErrorKun::Process()
{
    return SequencerBase::SequenceTypeEnum::E_SEQ_ERROR;
}

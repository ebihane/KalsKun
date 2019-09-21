#include "ErrorKun.h"

ErrorKun::ErrorKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_ERROR)
{

}

ErrorKun::~ErrorKun()
{

}

ResultEnum ErrorKun::initializeCore()
{
    return ResultEnum::NormalEnd;
}

void ErrorKun::destroyCore()
{

}

SequencerBase::SequenceTypeEnum ErrorKun::processCore()
{
    return SequencerBase::SequenceTypeEnum::E_SEQ_ERROR;
}

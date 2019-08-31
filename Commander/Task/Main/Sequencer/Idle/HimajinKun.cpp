#include "HimajinKun.h"

HimajinKun::HimajinKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_IDLE)
{

}

HimajinKun::~HimajinKun()
{

}

void HimajinKun::Initialize()
{

}

void HimajinKun::Destroy()
{

}

SequencerBase::SequenceTypeEnum HimajinKun::Process()
{
    return SequencerBase::SequenceTypeEnum::E_SEQ_IDLE;
}

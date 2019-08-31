#include "KusakariKun.h"

KusakariKun::KusakariKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_KUSAKARI)
{

}

KusakariKun::~KusakariKun()
{

}

void KusakariKun::Initialize()
{

}

void KusakariKun::Destroy()
{

}

SequencerBase::SequenceTypeEnum KusakariKun::Process()
{
    return SequencerBase::SequenceTypeEnum::E_SEQ_KUSAKARI;
}

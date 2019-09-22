#include "Parts/CommanderCommon.h"
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
    /* ��x���ł��邱�Ƃ������o�� */
    digitalWrite(IO_KUSATORI_MODE, LOW);
    digitalWrite(IO_YAKEI_MODE, HIGH);

    return SequencerBase::SequenceTypeEnum::E_SEQ_YAKEI;
}

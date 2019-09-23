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
    /* 夜警中であることを示す出力 */
    digitalWrite(IO_KUSATORI_MODE, LOW);
    digitalWrite(IO_YAKEI_MODE, HIGH);

    return SequencerBase::SequenceTypeEnum::E_SEQ_YAKEI;
}

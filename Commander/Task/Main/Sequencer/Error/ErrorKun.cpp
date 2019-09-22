#include "Parts/CommanderCommon.h"
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
    /* エラー状態出力を OFF */
    digitalWrite(IO_FAILURE_STATE, LOW);
}

SequencerBase::SequenceTypeEnum ErrorKun::processCore()
{
    SequenceTypeEnum retVal = MY_SEQUENCE_TYPE;

    /* エラー状態であることを示す出力 */
    digitalWrite(IO_FAILURE_STATE, HIGH);

    /* 別マイコンのシステム異常解除 */
    if ((pShareMemory->FrontCamera.SystemError != 0)
    ||  (pShareMemory->AroundCamera.SystemError != 0))
    {
        goto FINISH;
    }




    /* 全ての異常が解除されたら、直前のモードに戻る */
    retVal = m_BeforeSequence;

FINISH :
    return retVal;
}

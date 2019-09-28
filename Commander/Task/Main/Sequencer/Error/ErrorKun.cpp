#include "Parts/CommanderCommon.h"
#include "ErrorKun.h"

ErrorKun::ErrorKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_ERROR)
{
    /* nop. */
}

ErrorKun::~ErrorKun()
{
    /* Nop. */
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

    /* エラー音 */
    pShareMemory->Commander.MelodyMode = MelodyModeEnum::E_MELODY_ERROR;

    /* 点滅 */
    pShareMemory->Commander.LightMode = LightModeEnum::E_LIGHT_BLINK;

    /* 別マイコンのシステム異常発生中 */
    if ((pShareMemory->FrontCamera.SystemError != 0)
    ||  (pShareMemory->AnimalCamera.SystemError != 0)
    ||  (pShareMemory->AroundCamera.SystemError != 0))
    {
        goto FINISH;
    }

    /* 司令塔マイコンのシステム異常発生中 */
    if (pShareMemory->Commander.SystemError != false)
    {
        goto FINISH;
    }

    /* 全ての異常が解除されたら、直前のモードに戻る */
    retVal = m_BeforeSequence;

FINISH :
    return retVal;
}

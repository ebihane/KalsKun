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
    /* �G���[��ԏo�͂� OFF */
    digitalWrite(IO_FAILURE_STATE, LOW);
}

SequencerBase::SequenceTypeEnum ErrorKun::processCore()
{
    SequenceTypeEnum retVal = MY_SEQUENCE_TYPE;

    /* �G���[��Ԃł��邱�Ƃ������o�� */
    digitalWrite(IO_FAILURE_STATE, HIGH);

    /* �G���[�� */
    pShareMemory->Commander.MelodyMode = MelodyModeEnum::E_MELODY_ERROR;

    /* �_�� */
    pShareMemory->Commander.LightMode = LightModeEnum::E_LIGHT_BLINK;

    /* �ʃ}�C�R���̃V�X�e���ُ픭���� */
    if ((pShareMemory->FrontCamera.SystemError != 0)
    ||  (pShareMemory->AnimalCamera.SystemError != 0)
    ||  (pShareMemory->AroundCamera.SystemError != 0))
    {
        goto FINISH;
    }

    /* �i�ߓ��}�C�R���̃V�X�e���ُ픭���� */
    if (pShareMemory->Commander.SystemError != false)
    {
        goto FINISH;
    }

    /* �S�Ăُ̈킪�������ꂽ��A���O�̃��[�h�ɖ߂� */
    retVal = m_BeforeSequence;

FINISH :
    return retVal;
}

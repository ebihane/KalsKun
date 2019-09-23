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
    /* �G���[��ԏo�͂� OFF */
    digitalWrite(IO_FAILURE_STATE, LOW);
}

SequencerBase::SequenceTypeEnum ErrorKun::processCore()
{
    SequenceTypeEnum retVal = MY_SEQUENCE_TYPE;

    /* �G���[��Ԃł��邱�Ƃ������o�� */
    digitalWrite(IO_FAILURE_STATE, HIGH);

    /* �ʃ}�C�R���̃V�X�e���ُ���� */
    if ((pShareMemory->FrontCamera.SystemError != 0)
    ||  (pShareMemory->AroundCamera.SystemError != 0))
    {
        goto FINISH;
    }




    /* �S�Ăُ̈킪�������ꂽ��A���O�̃��[�h�ɖ߂� */
    retVal = m_BeforeSequence;

FINISH :
    return retVal;
}

#include <time.h>
#include <sys/time.h>
#include "Parts/ShareMemory/ShareMemory.h"
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
    SequenceTypeEnum retVal = E_SEQ_IDLE;
    struct timeval tmVal;
    struct tm* tmPtr = NULL;

    /* �ʃ}�C�R���ŃV�X�e���ُ픭�� */
    if ((pShareMemory->FrontCamera.SystemError != 0)
    || (pShareMemory->AroundCamera.SystemError != 0))
    {
        retVal = SequenceTypeEnum::E_SEQ_ERROR;
        goto FINISH;
    }

    /* ���ݎ������擾 */
    gettimeofday(&tmVal, 0);
    tmPtr = localtime(&tmVal.tv_sec);

    /* �w�肵�������ɂȂ��� */


FINISH :
    return retVal;
}

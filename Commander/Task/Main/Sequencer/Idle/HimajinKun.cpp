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

    /* 別マイコンでシステム異常発生 */
    if ((pShareMemory->FrontCamera.SystemError != 0)
    || (pShareMemory->AroundCamera.SystemError != 0))
    {
        retVal = SequenceTypeEnum::E_SEQ_ERROR;
        goto FINISH;
    }

    /* 現在時刻を取得 */
    gettimeofday(&tmVal, 0);
    tmPtr = localtime(&tmVal.tv_sec);

    /* 指定した時刻になった */


FINISH :
    return retVal;
}

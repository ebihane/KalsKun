#include "RemoteController.h"

RemoteController::RemoteController(AdapterBase* const adapter)
 : ReceiverThread(adapter)
{
    /* nop. */
}

RemoteController::~RemoteController()
{
    /* nop. */
}

ResultEnum RemoteController::initializeCore()
{
    m_RecvData = new char[8];
    return ResultEnum::NormalEnd;
}

bool RemoteController::isReceiveComplete(char* const buffer, const unsigned long size)
{
    bool retVal = false;
    unsigned long expectedLength = 0;

    /* �Œ�ł� 2 byte ��M���Ȃ��ƃR�}���h�͊m��ł��Ȃ� */
    if (size < 2)
    {
        retVal = false;
        goto FINISH;
    }

    if (buffer[1] == 0)
    {
        expectedLength = 8;
    }
    else if (buffer[1] == 1)
    {
        expectedLength = 5;
    }
    else if (buffer[2] == 10)
    {
        expectedLength = 3;
    }
    else if (buffer[2] == 20)
    {
        expectedLength = 2;
    }
    else
    {
        /* �ǂݎ̂Ă����邽�� 0 ���Z�b�g���� */
        expectedLength = 0;
    }

    if (size < expectedLength)
    {
        goto FINISH;
    }

    retVal = true;


FINISH :
    return retVal;
}

ResultEnum RemoteController::analyze(char* const buffer)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    /* ��M�f�[�^�`�F�b�N */
    if (buffer[0] != 0xFF)
    {
        /* �s���f�[�^��M */
        /* ���퉞�����邪�������Ȃ� = �ǂݎ̂� */
        retVal = ResultEnum::NormalEnd;
        goto FINISH;
    }

    switch (buffer[1])
    {
        case 0 :
            analyzeRemoteCommand(&buffer[2]);
            break;

        case 1 :
            analyzeSingleMotorDrive(&buffer[2]);
            break;

        case 10 :
            analyzeModeChange(&buffer[2]);
            break;

        case 20 :
            analyzeAlarmReset();
            break;

        default :
            break;
    }

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

void RemoteController::analyzeRemoteCommand(char* const buffer)
{

}

void RemoteController::analyzeSingleMotorDrive(char* const buffer)
{

}

void RemoteController::analyzeModeChange(char* const buffer)
{

}

void RemoteController::analyzeAlarmReset()
{

}

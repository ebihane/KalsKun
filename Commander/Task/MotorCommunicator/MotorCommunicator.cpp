#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "MotorCommunicator.h"

MotorCommunicator::MotorCommunicator(AdapterBase* const adapter)
 : LoopThreadBase((char*)"MotorComm", 100, TypeEnum::TIMER_STOP)
 , m_Queue(NULL)
 , m_Adapter(adapter)
 , m_LogFile(NULL)
 , m_IsOpen(false)
 , m_MotorCommand(MotorCommandEnum::E_COMMAND_STOP)
 , m_CutterMode(CutterDriveEnum::E_CUTTER_STOP)
{
    /* nop. */
}

MotorCommunicator::~MotorCommunicator()
{
    /* nop. */
}

ResultEnum MotorCommunicator::initializeCore()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    /* ��M�L���[���� */
    m_Queue = new Queue((char*)"MotorComm");
    if (m_Queue == NULL)
    {
        m_Logger->LOG_ERROR("[initializeCore] Queue allocation failed.\n");
        goto FINISH;
    }

    /* �L���[�I�[�v�� */
    if (m_Queue->Open() != ResultEnum::NormalEnd)
    {
        m_Logger->LOG_ERROR("[initializeCore] Queue Open failed.\n");
        goto FINISH;
    }

    /* ���O�t�@�C���폜 */
    remove("/home/pi/Commander/MotorLog.txt");

    /* ���O�t�@�C������ */
    m_LogFile = fopen("/home/pi/Commander/MotorLog.txt", "w");
    if (m_LogFile == NULL)
    {
        /* ���O�o���̂� */
        snprintf(&m_LogStr[0], sizeof(m_LogFile), "[initializeCore] Log File Open failed. errno[%d]\n", errno);
        m_Logger->LOG_ERROR(m_LogStr);
    }

    retVal = ResultEnum::NormalEnd;

FINISH:
    return retVal;
}

ResultEnum MotorCommunicator::doMainProc()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    bool receivable = false;
    char sendBuffer[3] = { 0 };
    char recvBuffer[8] = { 0 };
    EventInfo ev = { 0 };
    
    if (m_IsOpen == false)
    {
        /* ��������N���[�Y���� */
        m_Adapter->Close();

        /* �I�[�v�� */
        if (m_Adapter->Open() != ResultEnum::NormalEnd)
        {
            /* ���̎����Ń��g���C */
            if (m_Opened == true)
            {
                m_Logger->LOG_ERROR("[doMainProc] Open failed.\n");
            }
            goto COMM_ERROR;
        }

        /* �����������V���b�g���M */
        createSendData(MotorCommandEnum::E_COMMAND_STOP, CutterDriveEnum::E_CUTTER_STOP, &sendBuffer[0]);
        if (m_Adapter->Send(&sendBuffer[0], sizeof(sendBuffer)) != ResultEnum::NormalEnd)
        {
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[doMainProc] Send failed. errno[%d]\n", m_Adapter->GetLastError());
            m_Logger->LOG_ERROR(m_LogStr);
            goto COMM_ERROR;
        }

        if (m_Adapter->Receive(&recvBuffer[0], sizeof(recvBuffer)) != ResultEnum::NormalEnd)
        {
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[doMainProc] Receive failed. errno[%d]\n", m_Adapter->GetLastError());
            m_Logger->LOG_ERROR(m_LogStr);
            goto COMM_ERROR;
        }

        analyze(&recvBuffer[0]);

        m_IsOpen = true;
        m_Opened = true;
    }

    /* �C�x���g��M�L���[�m�F */
    if (m_Queue->IsReceivable(receivable) != ResultEnum::NormalEnd)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[doMainProc] Queue::IsReceivable failed. errno[%d]\n", m_Queue->GetLastError());
        m_Logger->LOG_ERROR(m_LogStr);
        goto FINISH;
    }

    /* �C�x���g���� */
    if (receivable == false)
    {
        /* �O��Ɠ����R�}���h�𓊂��� */
        createSendData(m_MotorCommand, m_CutterMode, &sendBuffer[0]);
    }
    /* �C�x���g�L�� */
    else
    {
        /* �C�x���g��M */
        if (m_Queue->Receive(&ev) != ResultEnum::NormalEnd)
        {
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[doMainProc] Queue::Receive failed. errno[%d]\n", m_Queue->GetLastError());
            m_Logger->LOG_ERROR(m_LogStr);
            goto FINISH;
        }

        switch (ev.Code)
        {
            case 0 :    /* 0: ����ύX */
                m_MotorCommand = (MotorCommandEnum)ev.lParam[0];
                if (m_MotorCommand != MotorCommandEnum::E_COMMAND_STOP)
                {
                    delay(2000);
                }
                break;
                
            case 1 :    /* 1: ������n����ύX */
                m_CutterMode = (CutterDriveEnum)ev.lParam[0];
                break;

            case 2 :    /* 2: ����E������n �����ύX */
                m_MotorCommand = (MotorCommandEnum)ev.lParam[0];
                m_CutterMode = (CutterDriveEnum)ev.lParam[1];
                break;

            default :
                goto FINISH;
        }

        createSendData(m_MotorCommand, m_CutterMode, &sendBuffer[0]);
    }

    /* ���[�^�}�C�R���ւ̑��M */
    outputLog(&sendBuffer[0], 3, 0);
    if (m_Adapter->Send(&sendBuffer[0], sizeof(sendBuffer)) != ResultEnum::NormalEnd)
    {
        snprintf(&m_LogStr[0], sizeof(m_LogStr), "[doMainProc] Send failed. errno[%d]\n", m_Adapter->GetLastError());
        m_Logger->LOG_ERROR(m_LogStr);
        goto FINISH;
    }

    /* ���[�^�}�C�R������̉�����M */
    if (receiveProc(&recvBuffer[0]) != ResultEnum::NormalEnd)
    {
        m_Logger->LOG_ERROR("[doMainProc] receiveProc failed.\n");
        goto FINISH;
    }

    analyze(&recvBuffer[0]);
    pShareMemory->Motor.CommunicationCount++;

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;

COMM_ERROR :
    m_IsOpen = false;
    pShareMemory->Motor.CommunicationError = DetectTypeEnum::DETECTED;

    /* �L���[���̃��b�Z�[�W��ǂݎ̂� */
    m_Queue->IsReceivable(receivable);
    if (receivable == true)
    {
        m_Queue->Receive(&ev);
    }

    return ResultEnum::NormalEnd;
}

ResultEnum MotorCommunicator::finalizeCore()
{
    /* ���[�^�}�C�R���ɕK����~�𓊂��Ă��� */
    if (m_IsOpen == true)
    {
        char sendBuffer[3] = { 0 };
        char recvBuffer[8] = { 0 };
        createSendData(MotorCommandEnum::E_COMMAND_STOP, CutterDriveEnum::E_CUTTER_STOP, &sendBuffer[0]);
        m_Adapter->Send(&sendBuffer[0], sizeof(sendBuffer));
        m_Adapter->Receive(&recvBuffer[0], sizeof(recvBuffer));
    }

    if (m_LogFile != NULL)
    {
        fclose(m_LogFile);
        m_LogFile = NULL;
    }

    if (m_Adapter != NULL)
    {
        m_Adapter->Close();
        delete m_Adapter;
        m_Adapter = NULL;
    }

    if (m_Queue != NULL)
    {
        m_Queue->Close();
        delete m_Queue;
        m_Queue = NULL;
    }

    return ResultEnum::NormalEnd;
}

ResultEnum MotorCommunicator::createSendData(const MotorCommandEnum command, const CutterDriveEnum cutter, char* const buffer)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    long tempCommand = 0;

    if (E_COMMAND_MAX <= command)
    {
        char log[40] = { 0 };
        snprintf(&log[0], sizeof(log), "[createSendData] Illegal command. [%d]\n", command);
        m_Logger->LOG_ERROR(log);
        goto FINISH;
    }

    if (E_CUTTER_TYPE_MAX <= cutter)
    {
        char log[40] = { 0 };
        snprintf(&log[0], sizeof(log), "[createSendData] Illegal cutter move. [%d]\n", cutter);
        m_Logger->LOG_ERROR(log);
        goto FINISH;
    }

    tempCommand = (long)cutter;
    tempCommand <<= 4;
    tempCommand |= ((long)command & 0x0000000F);

    buffer[0] = 0xFF;
    buffer[1] = 0x02;
    buffer[2] = (char)tempCommand;


FINISH :
    return retVal;
}

ResultEnum MotorCommunicator::analyze(char* const buffer)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    long tempCommand = 0;
    short tempDistance = 0;

    if (buffer[0] != 0xFF)
    {
        char log[40] = { 0 };
        snprintf(&log[0], sizeof(log), "[analyze] Illegal 1st Byte. [%02Xh]\n", buffer[0]);
        m_Logger->LOG_ERROR(log);
        goto FINISH;
    }

    if (buffer[1] != 0x01)
    {
        char log[40] = { 0 };
        snprintf(&log[0], sizeof(log), "[analyze] Illegal 2nd Byte. [%02Xh]\n", buffer[0]);
        m_Logger->LOG_ERROR(log);
        goto FINISH;
    }

    tempCommand = buffer[2] & 0x0F;
    pShareMemory->Motor.Command = (MotorCommandEnum)tempCommand;
    tempCommand = (buffer[2] >> 4) & 0x0F;
    pShareMemory->Motor.Cutter = (CutterDriveEnum)tempCommand;

    /* X/Y �͊�{��O�ی��ɂ��� */
    /* �W���C���̃f�[�^�� cm �P�ʂ̂��߁Amm �ɕϊ� */
    /* X �͍ŏ��Ɍ����Ă�������� - ���̂��߁A�����𔽓]����K�v���� */
    tempDistance = buffer[3];
    tempDistance = (short)(tempDistance << 8);
    tempDistance |= (short)buffer[4];
    pShareMemory->Motor.PointX = (tempDistance * -1) * 10;

    /* Y �͉E�^�[���� + �����Ȃ̂ŁA�����͂��̂܂܂� OK */
    tempDistance = buffer[5];
    tempDistance = (short)(tempDistance << 8);
    tempDistance |= (short)buffer[6];
    pShareMemory->Motor.PointY = tempDistance * 10;

    if ((buffer[7] & 0x01) != 0)
    {
        pShareMemory->Motor.ErrorStatus = DetectTypeEnum::DETECTED;
    }
    else
    {
        pShareMemory->Motor.ErrorStatus = DetectTypeEnum::NOT_DETECT;
    }

    if ((buffer[7] & 0x02) != 0)
    {
        pShareMemory->Motor.RemoteMode = E_MODE_MANUAL;
    }
    else
    {
        pShareMemory->Motor.RemoteMode = E_MODE_AUTO;
    }


FINISH :
    return retVal;
}

ResultEnum MotorCommunicator::receiveProc(char* const buffer)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    char once = 0;
    bool receivable = false;
    char log[80] = { 0 };
    Stopwatch watch;
    
    watch.Start();
    while (1)
    {
        if (3.0f <= watch.GetSplit())
        {
            m_Logger->LOG_ERROR("[receiveProc] Receive Timeout.\n");
            goto FINISH;
        }

        if (m_Adapter->IsReceivable(receivable) != ResultEnum::NormalEnd)
        {
            snprintf(&m_LogStr[0], sizeof(m_LogStr), "[receiveProc] IsReceivable failed. errno[%d]\n", m_Adapter->GetLastError());
            m_Logger->LOG_ERROR(m_LogStr);
            goto FINISH;
        }

        if (receivable == true)
        {
            if (m_Adapter->Receive(&once, 1) != ResultEnum::NormalEnd)
            {
                snprintf(&log[0], sizeof(log), "[receiveProc] Receive failed. errno[%d]\n", m_Adapter->GetLastError());
                m_Logger->LOG_ERROR(log);
                goto FINISH;
            }

            if (once == 0xFF)
            {
                buffer[0] = once;
                if (m_Adapter->Receive(&buffer[1], 7) != ResultEnum::NormalEnd)
                {
                    snprintf(&log[0], sizeof(log), "[receiveProc] Receive failed. errno[%d]\n", m_Adapter->GetLastError());
                    m_Logger->LOG_ERROR(log);
                    goto FINISH;
                }

                outputLog(&buffer[0], 8, 1);
                break;
            }
        }

        delay(1);
    }

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

void MotorCommunicator::outputLog(char* const buffer, const long size, const char type)
{
    char log[80] = { 0 };
    char once[8] = { 0 };

    if (m_LogFile == NULL)
    {
        goto FINISH;
    }

    if (type == 0)
    {
        snprintf(&log[0], sizeof(log), "[doMainProc] Send (%d) :", size);
    }
    else
    {
        snprintf(&log[0], sizeof(log), "[doMainProc] Recv (%d) :", size);
    }

    for (long index = 0; index < size; index++)
    {
        snprintf(&once[0], sizeof(once), " %02X", buffer[index]);
        strncat(&log[0], &once[0], sizeof(log));
    }

    fprintf(m_LogFile, "%s\n", &log[0]);

FINISH :
    return;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MotorCommunicator.h"

MotorCommunicator::MotorCommunicator()
 : LoopThreadBase((char*)"MotorComm", 100, TypeEnum::TIMER_STOP)
 , m_Queue(NULL)
 , m_Serial(NULL)
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
    Serial::SerialInfoStr serialSetting;
    Queue* queue = NULL;
    Serial* serial = NULL;
    char sendBuffer[3] = { 0 };
    char recvBuffer[8] = { 0 };


    /* ��M�L���[���� */
    queue = new Queue((char*)"MotorComm");
    if (queue == NULL)
    {
        m_Logger->LOG_ERROR("[initializeCore] Queue allocation failed.\n");
        goto FINISH;
    }

    /* �L���[�I�[�v�� */
    if (queue->Open() != ResultEnum::NormalEnd)
    {
        m_Logger->LOG_ERROR("[initializeCore] Queue Open failed.\n");
        goto FINISH;
    }

    /* �V���A���|�[�g�ݒ� */
    serialSetting.Baudrate = Serial::BaudrateEnum::E_Baudrate_115200;
    serialSetting.Parity = Serial::ParityEnum::E_Parity_Non;
    serialSetting.StopBit = Serial::StopBitEnum::E_StopBit_1Bit;
    serialSetting.DataLength = Serial::DataLengthEnum::E_Data_8bit;

    /* �V���A���ʐM�N���X���� */
    serial = new Serial((char*)"ttyUSB0", &serialSetting);
    if (serial == NULL)
    {
        m_Logger->LOG_ERROR("[initializeCore] Serial allocation failed.\n");
        goto FINISH;
    }

    /* �|�[�g�I�[�v�� */
    if (serial->Open() != ResultEnum::NormalEnd)
    {
        m_Logger->LOG_ERROR("[initializeCore] Serial Open failed.\n");
        goto FINISH;
    }

    /* �����������V���b�g���M */
    createSendData(MotorCommandEnum::E_COMMAND_STOP, CutterDriveEnum::E_CUTTER_STOP, &sendBuffer[0]);
    if (serial->Send(&sendBuffer[0], sizeof(sendBuffer)) != ResultEnum::NormalEnd)
    {
        char log[40] = { 0 };
        snprintf(&log[0], sizeof(log), "[doMainProc] Send failed. errno[%d]\n", serial->GetLastError());
        m_Logger->LOG_ERROR(log);
        goto FINISH;
    }

    if (serial->Receive(&recvBuffer[0], sizeof(recvBuffer)) != ResultEnum::NormalEnd)
    {
        char log[40] = { 0 };
        snprintf(&log[0], sizeof(log), "[doMainProc] Receive failed. errno[%d]\n", serial->GetLastError());
        m_Logger->LOG_ERROR(log);
        goto FINISH;
    }

    analyze(&recvBuffer[0]);

    m_Queue = queue;
    m_Serial = serial;

    retVal = ResultEnum::NormalEnd;

FINISH:

    if (retVal != ResultEnum::NormalEnd)
    {
        if (serial != NULL)
        {
            serial->Close();
            delete serial;
        }

        if (queue != NULL)
        {
            queue->Close();
            delete queue;
        }
    }

    return retVal;
}

ResultEnum MotorCommunicator::doMainProc()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    char sendBuffer[3] = { 0 };
    char recvBuffer[8] = { 0 };
    bool receivable = false;
    char log[80] = { 0 };
    EventInfo ev = { 0 };
    
    if (m_Queue->IsReceivable(receivable) != ResultEnum::NormalEnd)
    {
        snprintf(&log[0], sizeof(log), "[doMainProc] Queue::IsReceivable failed. errno[%d]\n", m_Queue->GetLastError());
        m_Logger->LOG_ERROR(log);
        goto FINISH;
    }

    if (receivable == true)
    {
        if (m_Queue->Receive(&ev) != ResultEnum::NormalEnd)
        {
            snprintf(&log[0], sizeof(log), "[doMainProc] Queue::Receive failed. errno[%d]\n", m_Queue->GetLastError());
            m_Logger->LOG_ERROR(log);
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
    else
    {
        createSendData(m_MotorCommand, m_CutterMode, &sendBuffer[0]);
    }

    outputLog(&sendBuffer[0], 3, 0);
    if (m_Serial->Send(&sendBuffer[0], sizeof(sendBuffer)) != ResultEnum::NormalEnd)
    {
        snprintf(&log[0], sizeof(log), "[doMainProc] Send failed. errno[%d]\n", m_Serial->GetLastError());
        m_Logger->LOG_ERROR(log);
        goto FINISH;
    }

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
}

ResultEnum MotorCommunicator::finalizeCore()
{
    /* ���[�^�}�C�R���ɕK����~�𓊂��Ă��� */
    char sendBuffer[3] = { 0 };
    char recvBuffer[8] = { 0 };
    createSendData(MotorCommandEnum::E_COMMAND_STOP, CutterDriveEnum::E_CUTTER_STOP, &sendBuffer[0]);
    m_Serial->Send(&sendBuffer[0], sizeof(sendBuffer));
    m_Serial->Receive(&recvBuffer[0], sizeof(recvBuffer));

    if (m_Serial != NULL)
    {
        m_Serial->Close();
        delete m_Serial;
        m_Serial = NULL;
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

    tempCommand = buffer[3];
    tempCommand <<= 8;
    tempCommand |= buffer[4];
    pShareMemory->Motor.PointX = tempCommand;

    tempCommand = buffer[5];
    tempCommand <<= 8;
    tempCommand |= buffer[6];
    pShareMemory->Motor.PointY = tempCommand;

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
    char log[80] = { 0 };


    while (1)
    {
        if (m_Serial->Receive(&once, 1) != ResultEnum::NormalEnd)
        {
            snprintf(&log[0], sizeof(log), "[receiveProc] Receive failed. errno[%d]\n", m_Serial->GetLastError());
            m_Logger->LOG_ERROR(log);
            goto FINISH;
        }

        if (once == 0xFF)
        {
            buffer[0] = once;
            if (m_Serial->Receive(&buffer[1], 7) != ResultEnum::NormalEnd)
            {
                snprintf(&log[0], sizeof(log), "[receiveProc] Receive failed. errno[%d]\n", m_Serial->GetLastError());
                m_Logger->LOG_ERROR(log);
                goto FINISH;
            }

            outputLog(&buffer[0], 8, 1);
            break;
        }
    }

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

void MotorCommunicator::outputLog(char* const buffer, const long size, const char type)
{
#if 0
    char log[80] = { 0 };
    char once[8] = { 0 };

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

    strncat(&log[0], "\n", sizeof(log));
    m_Logger->LOG_INFO(log);
#endif
}

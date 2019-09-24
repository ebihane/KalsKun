#include <vector>
#include "Parts/ShareMemory/ShareMemory.h"
#include "CameraSender.h"

CameraSender::CameraSender()
 : ThreadBase((char*)"CameraSender")
 , m_UdpSender(NULL)
 , m_SendBuffer(NULL)
{
    /* nop. */
}

CameraSender::~CameraSender()
{
    /* nop. */
}

ResultEnum CameraSender::initialize()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;


    m_SendBuffer = new char[UDP_SEND_MAX];
    if (m_SendBuffer == NULL)
    {
        goto FINISH;
    }

    m_UdpSender = new UdpSender((char *)"192.168.3.102", 10000, false);
    if (m_UdpSender == NULL)
    {
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;


FINISH :
    return retVal;
}

ResultEnum CameraSender::doProcedure()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    std::vector<unsigned char> vect;
    std::vector<int> param = std::vector<int>(2);

    if (m_UdpSender == NULL)
    {
        goto FINISH;
    }

    param[0] = IMWRITE_JPEG_QUALITY;
    param[1] = 85;

    pShareMemory->Communicate.Index = -1;

RECONNECT :

    retVal = m_UdpSender->Open();
    if (retVal != ResultEnum::NormalEnd)
    {
        m_Logger->LOG_ERROR("[CameraSender] UdpSender Open failed.\n");
        delay(5000);
        goto RECONNECT;
    }

    retVal = m_UdpSender->Connection();
    if (retVal != ResultEnum::NormalEnd)
    {
        m_Logger->LOG_ERROR("[CameraSender] UdpSender Connection failed.\n");
        delay(5000);
        goto RECONNECT;
    }

    m_Logger->LOG_INFO("[CameraSender] Main loop start.\n");
    while (1)
    {
        if (isStopRequest() == true)
        {
            break;
        }

        /* �摜���X�V����Ă��Ȃ��ԁA�ҋ@ */
        if (pShareMemory->Communicate.Index == pShareMemory->Capture.Index)
        {
            /* @todo : �����Ɉُ�n�̏��������� */
            delay(10);
            continue;
        }

        /* ���M�Ώۂ� Capture Data ���m�� */
        pShareMemory->Communicate.Index = pShareMemory->Capture.Index;

        /* jpeg �`���̉摜�f�[�^�ɕϊ� */
        Mat& sendMat = pShareMemory->Capture.Data[pShareMemory->Communicate.Index];
        imencode(".jpg", sendMat, vect, param);
        
        if (vect.size() < (UDP_SEND_MAX - sizeof(unsigned long)))
        {
            /* jpeg �摜�͉ϒ��̂��߁A�ŏ��� 4 byte �ɃT�C�Y���i�[���� */
            std::size_t size = vect.size();
            m_SendBuffer[0] = (char)((size & 0xFF000000) >> 24);
            m_SendBuffer[1] = (char)((size & 0x00FF0000) >> 16);
            m_SendBuffer[2] = (char)((size & 0x0000FF00) >> 8);
            m_SendBuffer[3] = (char) (size & 0x000000FF);
            for (unsigned long index = 0; index < vect.size(); index++)
            {
                m_SendBuffer[4 + index] = vect[index];
            }

            /* ���s���Ă��������đ��M�������� */
            m_UdpSender->Send(&m_SendBuffer[0], size + 4);
        }

        vect.clear();

        delay(10);
    }

    retVal = ResultEnum::NormalEnd;


FINISH :
    return retVal;
}

ResultEnum CameraSender::finalize()
{
    if (m_UdpSender != NULL)
    {
        m_UdpSender->Close();
    }

    if (m_SendBuffer != NULL)
    {
        delete[] m_SendBuffer;
        m_SendBuffer = NULL;
    }

    return ResultEnum::NormalEnd;
}

#include <vector>
#include "Parts/ShareMemory/ShareMemory.h"
#include "CameraSender.h"

CameraSender::CameraSender()
 : ThreadBase((char *)"CameraSender")
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

    m_UdpSender = new UdpSender((char *)"192.168.3.100", 10000, false);
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
    Mat resizeMat;
    Size resizeSize = Size(320, 240);
    long startCommand = 0;

    if (m_UdpSender == NULL)
    {
        goto FINISH;
    }

    param[0] = IMWRITE_JPEG_QUALITY;
    param[1] = 85;

    pShareMemory->Communicate.Index = -1;

RECONNECT :

    if (isStopRequest() == true)
    {
        retVal = ResultEnum::NormalEnd;
        goto FINISH;
    }

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

    retVal = m_UdpSender->Receive(&startCommand, sizeof(startCommand));
    if (retVal != ResultEnum::NormalEnd)
    {

    }

    m_Logger->LOG_INFO("[CameraSender] Main loop start.\n");
    while (1)
    {
        if (isStopRequest() == true)
        {
            break;
        }

        /* 画像が更新されていない間、待機 */
        if (pShareMemory->Communicate.Index == pShareMemory->Capture.Index)
        {
            /* @todo : ここに異常系の処理を入れる */
            delay(10);
            continue;
        }

        /* 送信対象の Capture Data を確定 */
        pShareMemory->Communicate.Index = pShareMemory->Capture.Index;

        /* jpeg 形式の画像データに変換 */
        Mat& sendMat = pShareMemory->Capture.Data[pShareMemory->Communicate.Index];
        resize(sendMat, resizeMat, resizeSize);
        imencode(".jpg", resizeMat, vect, param);
        
        if (vect.size() < (UDP_SEND_MAX - sizeof(unsigned long)))
        {
            /* jpeg 画像は可変長のため、最初の 4 byte にサイズを格納する */
            std::size_t size = vect.size();
            m_SendBuffer[0] = (char)((size & 0xFF000000) >> 24);
            m_SendBuffer[1] = (char)((size & 0x00FF0000) >> 16);
            m_SendBuffer[2] = (char)((size & 0x0000FF00) >> 8);
            m_SendBuffer[3] = (char) (size & 0x000000FF);
            for (unsigned long index = 0; index < vect.size(); index++)
            {
                m_SendBuffer[4 + index] = vect[index];
            }

            if (m_UdpSender->Send(&m_SendBuffer[0], size + 4) != ResultEnum::NormalEnd)
            {
                vect.clear();
                goto RECONNECT;
            }
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

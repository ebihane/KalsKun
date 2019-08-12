#include <vector>
#include "Parts/ShareMemory/ShareMemory.h"
#include "CameraReceiver.h"

CameraReceiver::CameraReceiver()
 : ThreadBase()
 , m_UdpReceiver(NULL)
 , m_ReceiveBuffer(NULL)
{
    /* nop. */
}

CameraReceiver::~CameraReceiver()
{
    /* nop. */
}

ResultEnum CameraReceiver::initialize()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    m_ReceiveBuffer = new char[UDP_RECEIVE_MAX];
    if (m_ReceiveBuffer == NULL)
    {
        m_Logger->LOG_ERROR("[initialize] ReceiveBuffer allocation failed.\n");
        pShareMemory->SystemError = true;
        goto FINISH;
    }

    m_UdpReceiver = new UdpReceiver(10000);
    if (m_UdpReceiver == NULL)
    {
        m_Logger->LOG_ERROR("[initialize] UdpReceiver allocation failed.\n");
        pShareMemory->SystemError = true;
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;

FINISH:
    return retVal;
}

ResultEnum CameraReceiver::doProcedure()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    ResultEnum result = ResultEnum::AbnormalEnd;
    bool receivable = false;
    long pictureSize = 0;
    long setIndex = 0;
    std::vector<unsigned char> vect;
    char* pBuffer = NULL;

    if (m_UdpReceiver == NULL)
    {
        m_Logger->LOG_ERROR("[CameraReceiver] UdpReceiver is NULL.\n");
        pShareMemory->SystemError = true;
        goto FINISH;
    }

    pShareMemory->Communicate.Index = -1;

RECONNECT :

    retVal = m_UdpReceiver->Open();
    if (retVal != ResultEnum::NormalEnd)
    {
        m_Logger->LOG_ERROR("[CameraReceiver] UdpReceiver Open failed.\n");
        delay(5000);
        goto RECONNECT;
    }

    retVal = m_UdpReceiver->Connection();
    if (retVal != ResultEnum::NormalEnd)
    {
        m_Logger->LOG_ERROR("[CameraReceiver] UdpReceiver Connection failed.\n");
        delay(5000);
        goto RECONNECT;
    }

    m_Logger->LOG_INFO("[CameraReceiver] Main loop start.\n");
    while (1)
    {
        result = m_UdpReceiver->IsReceivable(receivable);
        if (result != ResultEnum::NormalEnd)
        {
            if (isStopRequest() == true)
            {
                break;
            }

            if (result == ResultEnum::Reconnect)
            {
                goto RECONNECT;
            }
            else
            {
                m_Logger->LOG_ERROR("[CameraReceiver] Udp IsReceivable failed.\n");
                pShareMemory->SystemError = true;
                goto FINISH;
            }
        }

        if (receivable == false)
        {
            if (isStopRequest() == true)
            {
                break;
            }

            delay(10);
            continue;
        }

        // 最初に画像サイズを受信する
        result = m_UdpReceiver->Receive(&pictureSize, sizeof(pictureSize));
        if (result != ResultEnum::NormalEnd)
        {
            if (result == ResultEnum::Reconnect)
            {
                goto RECONNECT;
            }
            else
            {
                m_Logger->LOG_ERROR("[CameraReceiver] Udp Receive failed.\n");
                pShareMemory->SystemError = true;
                goto FINISH;
            }
        }

        if (pShareMemory->Communicate.Index < 0)
        {
            setIndex = 0;
        }
        else
        {
            setIndex = pShareMemory->Communicate.Index + 1;
            if (CAMERA_DATA_MAX <= setIndex)
            {
                setIndex = 0;
            }
        }

        // 画像本体を受信
        pBuffer = new char[pictureSize];
        result = m_UdpReceiver->Receive(&pBuffer[0], pictureSize);
        for (long index = 0; index < pictureSize; index++)
        {
            vect.push_back((unsigned char)pBuffer[index]);
        }

        delete[] pBuffer;

        // jpeg 画像にデコード
        pShareMemory->Communicate.Data[setIndex] = imdecode(Mat(vect), IMREAD_COLOR);
        pShareMemory->Communicate.Index = setIndex;

        vect.clear();
    }

    retVal = ResultEnum::NormalEnd;


FINISH :

    if (pBuffer != NULL)
    {
        delete[] pBuffer;
        pBuffer = NULL;
    }

    m_Logger->LOG_INFO("[CameraReceiver]::Exit.\n");
    return retVal;
}

ResultEnum CameraReceiver::finalize()
{
    if (m_UdpReceiver != NULL)
    {
        m_UdpReceiver->Close();
        delete m_UdpReceiver;
        m_UdpReceiver = NULL;
    }

    if (m_ReceiveBuffer != NULL)
    {
        delete[] m_ReceiveBuffer;
        m_ReceiveBuffer = NULL;
    }

    return ResultEnum::NormalEnd;
}

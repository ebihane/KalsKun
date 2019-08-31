#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include "Serial.h"

Serial::Serial(char* const name, SerialInfoStr* const setting)
 : m_Device(SERIAL_INVALID)
{
    if (setting != NULL)
    {
        memcpy(&m_Setting, setting, sizeof(SerialInfoStr));
    }
    else
    {
        m_Setting.Baudrate = E_Baudrate_9600;
        m_Setting.Parity = E_Parity_Non;
        m_Setting.StopBit = E_StopBit_1Bit;
        m_Setting.DataLength = E_Data_8bit;
    }

    snprintf(&m_Name[0], sizeof(m_Name), "/dev/%s", &name[0]);
}

Serial::~Serial()
{
    Close();
}

ResultEnum Serial::Open()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    int fd = SERIAL_INVALID;
    speed_t baudrate = convertBaudrate(m_Setting.Baudrate);
    struct termios setting;


    /* すでにオープン済みの場合は何もしない */
    if (m_Device != SERIAL_INVALID)
    {
        retVal = ResultEnum::NormalEnd;
        goto FINISH;
    }

    m_LastErrorNo = ERROR_NOTHING;
    fd = open(&m_Name[0], O_RDWR);
    if (fd < 0)
    {
        m_LastErrorNo = errno;
        goto FINISH;
    }

    if (tcgetattr(fd, &setting) < 0)
    {
        m_LastErrorNo = errno;
        close(fd);
        goto FINISH;
    }

    /*----------------*/
    /* c_cflag 設定群 */
    /*----------------*/

    /* 受信有効 */
    setting.c_cflag |= CREAD;

    /* モデム制御線 無視 */
    setting.c_cflag |= CLOCAL;

    /* データ長 */
    if (m_Setting.DataLength == E_Data_7bit)
    {
        setting.c_cflag |= CS7;
    }
    else
    {
        setting.c_cflag |= CS8;
    }

    /* ストップビット */
    if (m_Setting.StopBit == E_StopBit_2Bit)
    {
        setting.c_cflag |= CSTOPB;
    }

    /* ボーレート */
    cfsetispeed(&setting, baudrate);

    /* パリティ */
    if (m_Setting.Parity != E_Parity_Non)
    {
        if (m_Setting.Parity == E_Parity_Even)
        {
            setting.c_cflag |= PARENB;
            setting.c_cflag &= ~PARODD;
        }
        else
        {
            setting.c_cflag |= PARENB;
            setting.c_cflag |= PARODD;
        }
    }

    /*----------------*/
    /* c_iflag 設定群 */
    /*----------------*/

    /* カノニカルモード 無効 */
    setting.c_iflag &= ~ICANON;


    /* 設定反映 */
    if (tcsetattr(fd, TCSANOW, &setting) < 0)
    {
        m_LastErrorNo = errno;
        close(fd);
        goto FINISH;
    }

    m_Device = fd;

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

ResultEnum Serial::Close()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    if (m_Device == SERIAL_INVALID)
    {
        retVal = ResultEnum::NormalEnd;
        goto FINISH;
    }

    m_LastErrorNo = 0;
    if (close(m_Device) < 0)
    {
        /* errno を保持するが、ディスクリプタは初期化する */
        m_LastErrorNo = errno;
    }
    else
    {
        retVal = ResultEnum::NormalEnd;
    }

    m_Device = SERIAL_INVALID;
    

FINISH :
    return retVal;
}

ResultEnum Serial::Connection()
{
    ResultEnum retVal = Open();
    return retVal;
}

ResultEnum Serial::Disconnection()
{
    ResultEnum retVal = Close();
    return retVal;
}

ResultEnum Serial::IsSendable(bool& sendable)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    fd_set sendFds;
    fd_set errorFds;
    struct timeval timeout;

    sendable = false;

    if (m_Device == SERIAL_INVALID)
    {
        retVal = ResultEnum::Reconnect;
        goto FINISH;
    }

    FD_ZERO(&sendFds);
    FD_ZERO(&errorFds);
    FD_SET(m_Device, &sendFds);
    FD_SET(m_Device, &errorFds);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    m_LastErrorNo = ERROR_NOTHING;
    if (select(m_Device + 1, NULL, &sendFds, &errorFds, &timeout) < 0)
    {
        m_LastErrorNo = errno;
        retVal = ResultEnum::Reconnect;
    }
    else if (FD_ISSET(m_Device, &errorFds) != 0)
    {
        m_LastErrorNo = errno;
        retVal = ResultEnum::Reconnect;
    }
    else if (FD_ISSET(m_Device, &sendFds) == 0)
    {
        retVal = ResultEnum::NormalEnd;
    }
    else
    {
        sendable = true;
        retVal = ResultEnum::NormalEnd;
    }

FINISH:
    return retVal;
}

ResultEnum Serial::Send(void* const bufferPtr, const unsigned long size)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;


    m_LastErrorNo = ERROR_NOTHING;
    if (write(m_Device, bufferPtr, size) < 0)
    {
        m_LastErrorNo = errno;
        retVal = ResultEnum::Reconnect;
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;

FINISH:
    return retVal;
}

ResultEnum Serial::IsReceivable(bool& receivable)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    fd_set recvFds;
    fd_set errorFds;
    struct timeval  timeout;

    receivable = false;

    if (m_Device == SERIAL_INVALID)
    {
        retVal = ResultEnum::Reconnect;
        goto FINISH;
    }

    FD_ZERO(&recvFds);
    FD_ZERO(&errorFds);
    FD_SET(m_Device, &recvFds);
    FD_SET(m_Device, &errorFds);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    m_LastErrorNo = ERROR_NOTHING;
    if (select(m_Device + 1, &recvFds, NULL, &errorFds, &timeout) < 0)
    {
        m_LastErrorNo = errno;
        retVal = ResultEnum::Reconnect;
    }
    else if (FD_ISSET(m_Device, &errorFds) != 0)
    {
        m_LastErrorNo = errno;
        retVal = ResultEnum::Reconnect;
    }
    else if (FD_ISSET(m_Device, &recvFds) == 0)
    {
        retVal = ResultEnum::NormalEnd;
    }
    else
    {
        receivable = true;
        retVal = ResultEnum::NormalEnd;
    }

FINISH:
    return retVal;
}

ResultEnum Serial::Receive(void* const bufferPtr, const unsigned long size)
{
    ResultEnum   retVal = ResultEnum::AbnormalEnd;
    int onceReceive = 0;
    unsigned long rest = size;
    char* ptr = (char*)bufferPtr;

    if (m_Device == SERIAL_INVALID)
    {
        retVal = ResultEnum::Reconnect;
        goto FINISH;
    }

    m_LastErrorNo = ERROR_NOTHING;
    while (0 < rest)
    {
        onceReceive = read(m_Device, ptr, rest);
        if (onceReceive < 0)
        {
            m_LastErrorNo = errno;
            retVal = ResultEnum::Reconnect;
            goto FINISH;
        }

        ptr += onceReceive;
        rest -= onceReceive;
    }


    retVal = ResultEnum::NormalEnd;

FINISH:
    return retVal;
}

unsigned int Serial::convertBaudrate(const BaudrateEnum baudrate)
{
    speed_t retVal = B9600;

    switch (baudrate)
    {
        case E_Baudrate_9600   :    retVal = B9600;     break;
        case E_Baudrate_19200  :    retVal = B19200;    break;
        case E_Baudrate_38400  :    retVal = B38400;    break;
        case E_Baudrate_57600  :    retVal = B57600;    break;
        case E_Baudrate_115200 :    retVal = B115200;   break;
        case E_Baudrate_230400 :    retVal = B230400;   break;
        default :                   retVal = B9600;     break;
    }

    return retVal;
}

#pragma once

#include "Adapter/AdapterBase.h"

class Serial : public AdapterBase
{
public :
    
    static const int    SERIAL_INVALID = -1;
    static const long   NAME_MAX_LENGTH = 16;

    /* ボーレート指定 */
    typedef enum
    {
        E_Baudrate_9600 = 0,
        E_Baudrate_19200,
        E_Baudrate_38400,
        E_Baudrate_57600,
        E_Baudrate_115200,
        E_Baudrate_230400,
    } BaudrateEnum;

    /* パリティ指定 */
    typedef enum
    {
        E_Parity_Non = 0,
        E_Parity_Even,
        E_Parity_Odd,
    } ParityEnum;

    /* ストップビット指定 */
    typedef enum
    {
        E_StopBit_1Bit = 0,
        E_StopBit_2Bit,
    } StopBitEnum;

    /* データ長 */
    typedef enum
    {
        E_Data_8bit = 0,
        E_Data_7bit,
    } DataLengthEnum;

    /* シリアル通信設定構造体 */
    typedef struct
    {
        BaudrateEnum Baudrate;
        ParityEnum Parity;
        StopBitEnum StopBit;
        DataLengthEnum DataLength;
    } SerialInfoStr;

    Serial(char* const name, SerialInfoStr* const setting);
    virtual ~Serial();

    ResultEnum Open();
    ResultEnum Close();
    ResultEnum Connection();
    ResultEnum Disconnection();

    ResultEnum IsSendable(bool& sendable);
    ResultEnum Send(void* const bufferPtr, const unsigned long size);
    ResultEnum IsReceivable(bool& receivable);
    ResultEnum Receive(void* const bufferPtr, const unsigned long size);

protected :


private :

    int m_Device;
    char m_Name[NAME_MAX_LENGTH];
    SerialInfoStr m_Setting;

    unsigned int convertBaudrate(const BaudrateEnum baudrate);

};
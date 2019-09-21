#include <string.h>
#include <math.h>
#include "Parts/ShareMemory/ShareMemory.h"
#include "Parts/Setting/SettingManager.h"
#include "WavePowerMonitor.h"

WavePowerMonitor::WavePowerMonitor(const unsigned char apNo)
 : LoopThreadBase(200, TypeEnum::TIMER_STOP)
 , AP_INDEX(apNo)
 , m_Index(0)
{
    memset(&m_RssiArray[0], 0x00, sizeof(m_RssiArray));
    memset(&m_TxPowerArray[0], 0x00, sizeof(m_TxPowerArray));
}

WavePowerMonitor::~WavePowerMonitor()
{
    /* nop. */
}

ResultEnum WavePowerMonitor::initializeCore()
{
    ResultEnum  retVal = ResultEnum::AbnormalEnd;
    long rssi = 0;
    long txPower = 0;
    long rssiSum = 0;
    long txPowerSum = 0;
    unsigned char index = 0;


    /* �ŏ��� 10 ��d�g���x�𑪒肵�Ă��� */
    for (index = 0; index < AVERAGE_COUNT; index++)
    {
        /* todo : �d�g���x�f�[�^�擾 */


        m_RssiArray[index] = rssi;
        m_TxPowerArray[index] = txPower;

        /* ���Z */
        rssiSum += rssi;
        txPowerSum += txPower;

        delay(200);
    }

    /* ���ϒl�Z�o */
    pShareMemory->BeaconData[AP_INDEX].RssiAverage = (float)rssiSum / AVERAGE_COUNT;
    pShareMemory->BeaconData[AP_INDEX].TxPowerAverage = (float)txPowerSum / AVERAGE_COUNT;

    /* �ʒu���� */
    pShareMemory->BeaconData[AP_INDEX].Distance = calcurateDistance();

    retVal = ResultEnum::NormalEnd;


    return retVal;
}

ResultEnum WavePowerMonitor::doMainProc()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    long rssi = 0;
    long txPower = 0;
    long rssiSum = 0;
    long txPowerSum = 0;
    unsigned char index = 0;

    
    /* @todo : �d�g���x�f�[�^�擾 */


    /* �ŐV�̃f�[�^���Z�b�g���� */
    m_RssiArray[m_Index] = rssi;
    m_TxPowerArray[m_Index] = txPower;

    /* ���v�l�Z�o */
    for (index = 0; index < AVERAGE_COUNT; index++)
    {
        rssiSum += m_RssiArray[index];
        txPowerSum += m_TxPowerArray[index];
    }

    /* ���ω� */
    pShareMemory->BeaconData[AP_INDEX].RssiAverage = (float)rssiSum / AVERAGE_COUNT;
    pShareMemory->BeaconData[AP_INDEX].TxPowerAverage = (float)txPowerSum / AVERAGE_COUNT;

    /* �ʒu���� */
    pShareMemory->BeaconData[AP_INDEX].Distance = calcurateDistance();

    m_Index++;
    if (AVERAGE_COUNT <= m_Index)
    {
        m_Index = 0;
    }

    retVal = ResultEnum::NormalEnd;


    return retVal;
}

float WavePowerMonitor::calcurateDistance()
{
    /* �t���X�̓`�B���� */
    /* d = 10 ^ {(TxPower - RSSI) / (10 * �`���W��)} */
    SettingManager* setting = SettingManager::GetInstance();

    float retVal = 0.0f;
    float diff = pShareMemory->BeaconData[AP_INDEX].TxPowerAverage - pShareMemory->BeaconData[AP_INDEX].RssiAverage;
    float coeff = setting->GetWavePowerCoeff();

    retVal = pow(10.0f, diff / (coeff * 10.0f));

    return retVal;
}

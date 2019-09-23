#include "Parts/ShareMemory/ShareMemory.h"
#include "Parts/FrontCameraCommon.h"
#include "UltrasoundManager.h"
#include <stdio.h>

UltrasoundManager::UltrasoundManager(const int sensorNo)
 : LoopThreadBase(100, TypeEnum::CYCLIC)
 , m_SensorNo(sensorNo)
{
    /* nop. */
}

UltrasoundManager::~UltrasoundManager()
{
    /* nop. */
}

ResultEnum UltrasoundManager::doMainProc()
{
    int triggerPin = 0;
    int echoPin = 0;

    if (m_SensorNo == 0)
    {
        triggerPin = IO_TRIGGER1_PIN;
        echoPin = IO_ECHO1_PIN;
    }
    else
    {
        triggerPin = IO_TRIGGER2_PIN;
        echoPin = IO_ECHO2_PIN;
    }

    // �����g�o�͊J�n
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    double duration = pulseIn(echoPin);
    if (duration > 0.0f)
    {
        duration = duration / 2;                    //���������𔼕��ɂ���
        pShareMemory->UltrasoundData[m_SensorNo] = (float)(duration * 340 * 100 / 1000000);  // ������340m/s�ɐݒ�
    }

    return ResultEnum::NormalEnd;
}

double UltrasoundManager::pulseIn(int pinNo)
{
    double retVal = -1.0f;
    Stopwatch highWatch;
    Stopwatch lowWatch;

    lowWatch.Start();
    while (1)
    {
        double time = lowWatch.GetMicro();
        if (time >= 1000000)
        {
            retVal = -2.0f;
            goto FINISH;
        }

        long signal = digitalRead(pinNo);
        if (signal == LOW)
        {
            break;
        }
    }

    highWatch.Start();
    while (1)
    {
        double time = highWatch.GetMicro();
        if (time >= 1000000)
        {
            retVal = -3.0f;
            goto FINISH;
        }

        long signal = digitalRead(pinNo);
        if (signal == HIGH)
        {
            retVal = time;
            break;
        }
    }

FINISH:
    return retVal;
}


#include "Parts/ShareMemory/ShareMemory.h"
#include "Parts/FrontCameraCommon.h"
#include "UltrasoundManager.h"
#include <stdio.h>

UltrasoundManager::UltrasoundManager(const int sensorNo)
 : LoopThreadBase((char*)"Ultrasound", 100, TypeEnum::TIMER_STOP)
 , m_SensorNo(sensorNo)
{
    pShareMemory->UltrasoundData[m_SensorNo] = -1;
}

UltrasoundManager::~UltrasoundManager()
{
    /* nop. */
}

ResultEnum UltrasoundManager::doMainProc()
{
    int triggerPin = 0;
    int echoPin = 0;
    float durationTime = -1.0f;
    struct timespec startTime;
    struct timespec stopTime;
    __time_t secondDiff;
    __time_t nanosecDiff;
    Stopwatch watch;

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
    
RETRY :
    // í¥âπîgèoóÕäJén
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    watch.Start();

    while (1)
    {
        long signal = digitalRead(echoPin);
        if (signal == HIGH)
        {
            break;
        }

        if (1.0f <= watch.GetSplit())
        {
            if (isStopRequest() == true)
            {
                goto FINISH;
            }

            delay(10);
            goto RETRY;
        }

        delayMicroseconds(1);
    }

    clock_gettime(CLOCK_REALTIME, &startTime);
    while (1)
    {
        long signal = digitalRead(echoPin);
        if (signal == LOW)
        {
            clock_gettime(CLOCK_REALTIME, &stopTime);
            secondDiff = stopTime.tv_sec - startTime.tv_sec;
            if (startTime.tv_nsec <= stopTime.tv_nsec)
            {
                nanosecDiff = stopTime.tv_nsec - startTime.tv_nsec;
            }
            else
            {
                nanosecDiff = (stopTime.tv_nsec + 1000000000) - startTime.tv_nsec;
                secondDiff--;
            }
            durationTime = ((float)secondDiff * 1000000) + ((float)nanosecDiff / 1000);

            break;
        }

        if (1.0f <= watch.GetSplit())
        {
            if (isStopRequest() == true)
            {
                goto FINISH;
            }

            delay(10);
            goto RETRY;
        }

        delayMicroseconds(1);
    }

    if (durationTime > 0)
    {
        durationTime = durationTime / 2;                    //âùïúãóó£Çîºï™Ç…Ç∑ÇÈ
        pShareMemory->UltrasoundData[m_SensorNo] = (float)(durationTime * 340 * 100 / 1000000);  // âπë¨Ç340m/sÇ…ê›íË
    }

FINISH:
    return ResultEnum::NormalEnd;
}



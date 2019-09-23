#include <float.h>
#include "Stopwatch.h"

Stopwatch::Stopwatch()
 : m_Running(false)
{
    /* nop. */
}

Stopwatch::~Stopwatch()
{
    /* nop. */
}

void Stopwatch::Start()
{
    clock_gettime(CLOCK_REALTIME, &m_StartTime);
    m_Running = true;
}

void Stopwatch::Stop()
{
    clock_gettime(CLOCK_REALTIME, &m_StopTime);
    m_Running = false;
}

bool Stopwatch::IsRunninng()
{
    return m_Running;
}

float Stopwatch::Get()
{
    float retVal = FLT_MIN;

    if (m_Running != false)
    {
        goto FINISH;
    }

    retVal = calcDiffTime(&m_StopTime);
    
FINISH :
    return retVal;
}

double Stopwatch::GetMicro()
{
    double retVal = FLT_MIN;

    if (m_Running != false)
    {
        goto FINISH;
    }

    retVal = calcDiffMicroTime(&m_StopTime);

FINISH:
    return retVal;
}

float Stopwatch::GetSplit()
{
    float retVal = FLT_MIN;
    struct timespec nowTime;
    
    if (m_Running != true)
    {
        goto FINISH;
    }

    clock_gettime(CLOCK_REALTIME, &nowTime);
    retVal = calcDiffTime(&nowTime);

FINISH :
    return retVal;
}

float Stopwatch::calcDiffTime(struct timespec* target)
{
    float retVal = 0.0F;
    unsigned long secondDiff = 0;
    unsigned long nanosecDiff = 0;

    if (m_Running != true)
    {
        retVal = FLT_MIN;
        goto FINISH;
    }

    secondDiff = target->tv_sec - m_StartTime.tv_sec;

    if (m_StartTime.tv_nsec <= target->tv_nsec)
    {
        nanosecDiff = target->tv_nsec - m_StartTime.tv_nsec;
    }
    else
    {
        nanosecDiff = (target->tv_nsec + SEC_TO_NSEC) - m_StartTime.tv_nsec;
        secondDiff--;
    }

    retVal = (float)secondDiff + ((float)nanosecDiff / (float)SEC_TO_NSEC);

FINISH :
    return retVal;
}

double Stopwatch::calcDiffMicroTime(struct timespec* target)
{
    double retVal = 0.0;
    unsigned long secondDiff = 0;
    unsigned long nanosecDiff = 0;

    if (m_Running != true)
    {
        retVal = FLT_MIN;
        goto FINISH;
    }

    secondDiff = target->tv_sec - m_StartTime.tv_sec;

    if (m_StartTime.tv_nsec <= target->tv_nsec)
    {
        nanosecDiff = target->tv_nsec - m_StartTime.tv_nsec;
    }
    else
    {
        nanosecDiff = (target->tv_nsec + SEC_TO_NSEC) - m_StartTime.tv_nsec;
        secondDiff--;
    }

    retVal = (double)secondDiff + ((double)nanosecDiff / (double)SEC_TO_NSEC) * 1000000;

FINISH:
    return retVal;
}

#pragma once

#include <time.h>
#include "Include/Common.h"

class Stopwatch
{
public :

    Stopwatch();
    virtual ~Stopwatch();

    void Start();
    void Stop();
    bool IsRunninng();
    float Get();
    float GetSplit();

protected :


private :

    static const unsigned long SEC_TO_NSEC = 1000 * 1000 * 1000;

    bool m_Running;
    struct timespec m_StartTime;
    struct timespec m_StopTime;

    float calcDiffTime(struct timespec* target);

};
#include <math.h>
#include "TudeConverter.h"

TudeConverter::TudeConverter()
{
    /* nop. */
}

TudeConverter::~TudeConverter()
{
    /* nop. */
}

/* “ñ‚Â‚ÌˆÜŒoŠÔ‚Ì‹——£‚ðŠ·ŽZ‚·‚é */
double TudeConverter::ToDistance(TudeStr* const base, TudeStr* const target)
{
    double retVal = 0.0;
    double tempLatitude = 0.0;
    double tempLongitude = 0.0;
    double latitudeDiff = 0.0;
    double longitudeDiff = 0.0;

    tempLatitude = degreeToRadian(base->Latitude - target->Latitude);
    tempLongitude = degreeToRadian(base->Longitude - target->Longitude);

    latitudeDiff = EARTH_R * tempLatitude;
    longitudeDiff = cos(degreeToRadian(base->Latitude)) * EARTH_R * tempLongitude;

    retVal = sqrt(pow(latitudeDiff, 2) + pow(longitudeDiff, 2));

    return retVal;
}

double TudeConverter::degreeToRadian(const double degree)
{
    double retVal = 0.0;
    double temp = 0.0;

    temp = degree / M_PI;
    temp *= 180.0;

    retVal = temp;

    return retVal;
}

double TudeConverter::radianToDegree(const double radian)
{
    double retVal = 0.0;
    double temp = 0.0;

    temp = radian / 180.0;
    temp *= M_PI;

    retVal = temp;

    return retVal;
}

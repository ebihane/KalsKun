#pragma once

class TudeConverter
{
public :
    
    typedef struct
    {
        double Latitude;    /* 緯度 */
        double Longitude;   /* 経度 */
    } TudeStr;

    TudeConverter();
    virtual ~TudeConverter();

    /* 二つの緯経間の距離を換算する */
    double ToDistance(TudeStr* const base, TudeStr* const target);

protected :

private :

    /* 地球半径 */
    const double EARTH_R;

    /* 角度 → ラジアン 変換 */
    double degreeToRadian(const double degree);

    /* ラジアン → 角度 変換 */
    double radianToDegree(const double radian);
};

#pragma once

class TudeConverter
{
public :
    
    typedef struct
    {
        double Latitude;    /* �ܓx */
        double Longitude;   /* �o�x */
    } TudeStr;

    TudeConverter();
    virtual ~TudeConverter();

    /* ��̈܌o�Ԃ̋��������Z���� */
    double ToDistance(TudeStr* const base, TudeStr* const target);

protected :

private :

    /* �n�����a */
    const double EARTH_R;

    /* �p�x �� ���W�A�� �ϊ� */
    double degreeToRadian(const double degree);

    /* ���W�A�� �� �p�x �ϊ� */
    double radianToDegree(const double radian);
};

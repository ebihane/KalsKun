#include "ThreadBase/LoopThreadBase.h"

class UltrasoundManager : public LoopThreadBase
{
public :

    UltrasoundManager(const int sensorNo);
    virtual ~UltrasoundManager();

protected :


private :

    int m_SensorNo;

    ResultEnum doMainProc();
    double pulseIn(int pinNo);

};

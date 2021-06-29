#ifndef RSutley2016MorbidityEstimator_H
#define RSutley2016MorbidityEstimator_H

#include "RObject.h"
#include <math.h>

class RDomain;
class RRandomVariable;

class RSutley2016MorbidityEstimator : public RObject
{

    Q_OBJECT

public:

    RSutley2016MorbidityEstimator(QObject *parent, QString name);
    ~RSutley2016MorbidityEstimator();

    void setIncome(QString value);
    void setBuildingType(QString value);
    void setTime(double value);
    void setDamageState(int value);

    bool estimateFatalities();


protected:


private:

    QString theIncome;
    QString theBuildingType;
    double theTime;
    int theDS;

};

#endif

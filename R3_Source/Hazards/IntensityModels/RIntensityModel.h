#ifndef RIntensityModel_H
#define RIntensityModel_H	

#include "RModel.h"	

class RParameter;	
class RResponse;
class RLocation;

class RIntensityModel : public RModel
{	
    Q_OBJECT
    Q_ENUMS(RResponseType)

public:	
    RIntensityModel(QObject *parent, QString name);
    ~RIntensityModel();

    int resetTime();

    void setPeriod(double value);

    QObject * getLocation();
    void setLocation(QObject *value);

    double getRjb();

    void setIntensityMeasure(QString theIntensityMeasure);

    virtual bool isOn() = 0;

    enum RResponseType{PGA, PGD, PGV, Sa};

protected:

    double thePeriod;
    double theRjb;
    QPointer<RLocation> theLocation;
    RResponseType theResponseType;
};

#endif

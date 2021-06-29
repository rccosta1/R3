#ifndef RGivenIntensityModel_H
#define RGivenIntensityModel_H

#include "RContinuousRandomVariable.h"
#include "RLinearSolver.h"
#include "REarthquakeBaseClass.h"

class RParameter;
class RResponse;
class RLocation;

class RGivenIntensityModel : public REarthquakeBaseClass
{
    Q_OBJECT

    Q_PROPERTY(QString MagnitudeList READ getMagnitudeList WRITE setMagnitudeList)
    Q_PROPERTY(QString HypocentreList READ getHypocentreList WRITE setHypocentreList)
    Q_PROPERTY(double FixedIntensity READ getFixedIntensity WRITE setFixedIntensity)


public:
    RGivenIntensityModel(QObject *parent, QString name);
    ~RGivenIntensityModel();

    int resetTime();

    QString getMagnitudeList();
    void setMagnitudeList(QString value);

    QString getHypocentreList();
    void setHypocentreList(QString value);

    double getFixedIntensity();
    void setFixedIntensity(double value);


    bool isOn();
    int evaluateModel();

private:

    double theGivenIntensity;
    QList<QPointer<RParameter> > theMagnitudeList;
    QList<QPointer<RParameter> > theHypocentreList;
    RResponse *theResponse = nullptr;

};

#endif

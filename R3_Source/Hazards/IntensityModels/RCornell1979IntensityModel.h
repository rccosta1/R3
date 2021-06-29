#ifndef RCornell1979IntensityModel_H
#define RCornell1979IntensityModel_H

#include "RContinuousRandomVariable.h"
#include "RLinearSolver.h"
#include "REarthquakeBaseClass.h"

class RParameter;	
class RResponse;
class RLocation;

class RCornell1979IntensityModel : public REarthquakeBaseClass
{	
    Q_OBJECT

    Q_PROPERTY(QString MagnitudeList READ getMagnitudeList WRITE setMagnitudeList)
    Q_PROPERTY(QString HypocentreList READ getHypocentreList WRITE setHypocentreList)


public:	
    RCornell1979IntensityModel(QObject *parent, QString name);
    ~RCornell1979IntensityModel();

    int resetTime();

    QString getMagnitudeList();
    void setMagnitudeList(QString value);

    QString getHypocentreList();
    void setHypocentreList(QString value);


    bool isOn();
    int evaluateModel();

private:

    QList<QPointer<RParameter> > theMagnitudeList;
    QList<QPointer<RParameter> > theHypocentreList;
    RResponse *theResponse = nullptr;

};	

#endif

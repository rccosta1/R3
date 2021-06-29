#ifndef RPGDIntensityModel_H
#define RPGDIntensityModel_H

#include "RModel.h"
#include "RContinuousRandomVariable.h"
#include "RLinearSolver.h"
#include "RIntensityModel.h"

class RParameter;
class RResponse;
class RLocation;

class RPGDIntensityModel : public RIntensityModel
{
    Q_OBJECT

    Q_PROPERTY(QString MagnitudeList READ getMagnitudeList WRITE setMagnitudeList)
    Q_PROPERTY(QString HypocentreList READ getHypocentreList WRITE setHypocentreList)
    Q_PROPERTY(QObject *PeakGroundAcceleration READ getPeakGroundAcceleration WRITE setPeakGroundAcceleration)

public:

    RPGDIntensityModel(QObject *parent, QString name);
    ~RPGDIntensityModel();

    int resetTime();

    QString getMagnitudeList();
    void setMagnitudeList(QString value);

    QString getHypocentreList();
    void setHypocentreList(QString value);

    QObject *getPeakGroundAcceleration() const;
    void setPeakGroundAcceleration(QObject *value);

    double getProbabilityOfLiquefaction();

    double getLateralPGD();

    void setSoilSusceptibilityLevel(QString name);

    void getLandslideSuceptibility(QString geologicalType, bool dry, double angle);

    double getLandslidePGD();

    bool isOn();
    int evaluateModel();

protected:



private:
    // Liquefaction parameters
    double theLiquefactionSusceptibility;
    double theProbabilityOfLiquefaction;
    double theVerticalPGD;
    double theLateralPGD;
    double theExpectedPGD;

    // landslide parameters
    int theLandslideSusceptibility;
    double cprime;
    double phiprime;
    double minimumSlopeAngle;
    double minimumCriticalAcceleration;

    QList<QPointer<RParameter> > theMagnitudeList;
    QList<QPointer<RParameter> > theHypocentreList;
    QPointer<RParameter> thePeakGroundAcceleration;
    QString theSoilSusceptibilityLevel;

    RResponse *theResponse;
};

#endif

#ifndef RSimpleBuildingDamageModel_H
#define RSimpleBuildingDamageModel_H

#include "RModel.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"
class RParameter;
class RResponse;

class RSimpleBuildingDamageModel : public RModel
{
    Q_OBJECT

    Q_PROPERTY(QString BuildingType READ getBuildingType WRITE setBuildingType)
    Q_PROPERTY(QString CodeDesignLevel READ getCodeDesignLevel WRITE setCodeDesignLevel)
    Q_PROPERTY(QObject *Location READ getLocation WRITE setLocation)
    Q_PROPERTY(QObject *PeakDrift READ getPeakDrift WRITE setPeakDrift)
    Q_PROPERTY(QObject *PeakAcceleration READ getPeakAcceleration WRITE setPeakAcceleration)
    Q_PROPERTY(QObject *ResidualDrift READ getResidualDrift WRITE setResidualDrift)
    Q_PROPERTY(int NumberOfStoreys READ getNumberOfStoreys WRITE setNumberOfStoreys)


public:
    RSimpleBuildingDamageModel(QObject *parent, QString name);
    ~RSimpleBuildingDamageModel();

    QObject *getPeakDrift() const;
    void setPeakDrift(QObject *value);

    QObject *getPeakAcceleration() const;
    void setPeakAcceleration(QObject *value);

    QObject *getResidualDrift() const;
    void setResidualDrift(QObject *value);

    QObject *getLocation() const;
    void setLocation(QObject *value);

    QString getBuildingType();
    void setBuildingType(QString value);

    QString getCodeDesignLevel();
    void setCodeDesignLevel(QString value);

    int getNumberOfStoreys() const;
    void setNumberOfStoreys(int value);

    QString getOccupancyClass() const;
    void setOccupancyClass(QString value);

    QString getTenure() const;
    void setTenure(QString value);

    double getEffectiveDamageRatio();
    double getStrDamageRatio();
    double getNSDDamageRatio();
    double getNSADamageRatio();
    double getContentDamageRatio();

    int evaluateModel();

private:



    RResponse *thePeakDrift;
    RResponse *thePeakAcceleration;
    RResponse *theResidualDriftResponse;
    RParameter *theLocation;
    QString theBuildingType;
    QString theOccupancyClass;
    QString theTenure;
    QString theCodeDesignLevel;
    int theNumberOfStoreys;


    QPointer<RContinuousRandomVariable> theDamageTheta1;
    QPointer<RContinuousRandomVariable> theDamageTheta2;
    QPointer<RContinuousRandomVariable> theDamageTheta3;
    QPointer<RContinuousRandomVariable> theDamageTheta4;
    QPointer<RContinuousRandomVariable> theDamageTheta5;
    QPointer<RContinuousRandomVariable> theDamageTheta6;
    QPointer<RContinuousRandomVariable> theDamageTheta7;
    QPointer<RContinuousRandomVariable> theDamageTheta8;
    QPointer<RContinuousRandomVariable> theDamageTheta9;
    QPointer<RContinuousRandomVariable> theDamageTheta10;
    QPointer<RContinuousRandomVariable> theDamageTheta11;
    QPointer<RContinuousRandomVariable> theDamageTheta12;
    QPointer<RContinuousRandomVariable> theDamageTheta13;
    QPointer<RContinuousRandomVariable> theDamageTheta14;
    QPointer<RContinuousRandomVariable> theDamageTheta15;
    QPointer<RContinuousRandomVariable> theDamageEpsilon1;
    QPointer<RContinuousRandomVariable> theDamageEpsilon2;
    QPointer<RContinuousRandomVariable> theDamageEpsilon3;
    QPointer<RContinuousRandomVariable> theDamageEpsilon4;

    RResponse *theStructuralDamageRatio;
    RResponse *theNonstructuralDriftDamageRatio;
    RResponse *theNonstructuralAccelerationDamageRatio;
    RResponse *theEffectiveDamageRatio;
    RResponse *theContentDamageRatio;
};

#endif

#ifndef RSimpleBuildingResponseModel_H
#define RSimpleBuildingResponseModel_H

#include "RModel.h"
#include "RContinuousRandomVariable.h"

class RParameter;
class RResponse;

class RSimpleBuildingResponseModel : public RModel
{
    Q_OBJECT

    Q_PROPERTY(QObject *Sa READ getSa WRITE setSa)
    Q_PROPERTY(QObject *Location READ getLocation WRITE setLocation)
    Q_PROPERTY(QString BuildingType READ getBuildingType WRITE setBuildingType)
    Q_PROPERTY(QString CodeDesignLevel READ getCodeDesignLevel WRITE setCodeDesignLevel)
    Q_PROPERTY(int NumberOfStoreys READ getNumberOfStoreys WRITE setNumberOfStoreys)

public:

    RSimpleBuildingResponseModel(QObject *parent, QString name);
    ~RSimpleBuildingResponseModel();

    QObject *getSa() const;
    void setSa(QObject *value);

//    QString getIntensityList();
//    void setIntensityList(QString value);

    QObject *getLocation() const;
    void setLocation(QObject *value);

    QString getBuildingType();
    void setBuildingType(QString value);

    QString getCodeDesignLevel();
    void setCodeDesignLevel(QString value);

    int getNumberOfStoreys() const;
    void setNumberOfStoreys(int value);

    int evaluateModel();

private:

    QPointer<RParameter> theSa;
    QPointer<RParameter> theLocation;
    QString theBuildingType;
    QString theCodeDesignLevel;
    int theNumberOfStoreys;
    QString theSaList;


    QPointer<RContinuousRandomVariable> theResponseTheta1;
    QPointer<RContinuousRandomVariable> theResponseTheta2;
    QPointer<RContinuousRandomVariable> theResponseTheta3;
    QPointer<RContinuousRandomVariable> theResponseTheta4;
    QPointer<RContinuousRandomVariable> theResponseTheta5;
    QPointer<RContinuousRandomVariable> theResponseTheta6;
    QPointer<RContinuousRandomVariable> theResponseTheta7;
    QPointer<RContinuousRandomVariable> theResponseTheta8;
    QPointer<RContinuousRandomVariable> theResponseTheta9;
    QPointer<RContinuousRandomVariable> theResponseTheta10;
    QPointer<RContinuousRandomVariable> theResponseTheta11;
    QPointer<RContinuousRandomVariable> theResponseTheta12;
    QPointer<RContinuousRandomVariable> theResponseTheta13;
    QPointer<RContinuousRandomVariable> theResponseTheta14;
    QPointer<RContinuousRandomVariable> theResponseTheta15;
    QPointer<RContinuousRandomVariable> theResponseTheta16;
    QPointer<RContinuousRandomVariable> theResponseTheta17;
    QPointer<RContinuousRandomVariable> theResponseTheta18;
    QPointer<RContinuousRandomVariable> theResponseTheta19;
    QPointer<RContinuousRandomVariable> theResponseTheta20;
    QPointer<RContinuousRandomVariable> theResponseTheta21;
    QPointer<RContinuousRandomVariable> theResponseSigma1;
    QPointer<RContinuousRandomVariable> theResponseSigma2;
    QPointer<RContinuousRandomVariable> theResponseEpsilon1;
    QPointer<RContinuousRandomVariable> theResponseEpsilon2;


    RResponse *thePeakDriftResponse;
    RResponse *thePeakAccelerationResponse;
    RResponse *theResidualDriftRatioResponse;
};

#endif

#ifndef RStorageTankModel_H
#define RStorageTankModel_H

#include "RModel.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RObject.h"


class RStorageTankModel : public RObject
{

public:

    RStorageTankModel(QObject *parent, QString name);
    ~RStorageTankModel();

    void setFragilityCurves(double value);

    QString getIntensityList();
    void setIntensityList(QString value);

    int evaluateModel(RGradientType theGradientType);

protected:

    double theFillLevel;

    // METHODS
    int calculateIntensityAndDamage();

    // INPUT TO DAMAGE MODEL
    QVector<double> SaDamageMedians;
    QVector<double> SaDamageDispersions;

    QVector<double> PGADamageMedians;
    QVector<double> PGADamageDispersions;

    QVector<double> PGVDamageMedians;
    QVector<double> PGVDamageDispersions;

    QVector<double> PGDDamageMedians;
    QVector<double> PGDDamageDispersions;

    QVector<double> restorationMeans;
    QVector<double> restorationStdvs;
    QVector<double> damageRatios;

    // RESULTS FROM THE ANALYSIS
    double PGAIntensity;
    double SaIntensity;
    double PGVIntensity;
    double PGDIntensity;
    QVector<double> PGAProbabilityVector, SaProbabilityVector, PGVProbabilityVector, PGDProbabilityVector, probabilityVector;
    double timeOfEvent;
    double meanRestorationRatio;
    double meanDamageRatio;

private:

    double theTankCapacity;

    // METHODS
    int calculateIntensity();
    int calculateDamageProbabilities();
    double calculateMeanDamageRatio();
    double calculateMeanRestorationRatio();

    // PROPERTIES
    //QPointer<RParameter> theLocation;
    QList<QPointer<RParameter> > theIntensityList;
    QList<QPointer<RParameter> > theClosures;

};

#endif

#ifndef RWaterDemandZoneModel_H
#define RWaterDemandZoneModel_H

#include "RModel.h"
#include "RNetworkModel.h"
#include "RLocation.h"
#include "RProbabilityDistributions.h"
#include "RHouseholdPortfolio.h"
#include "RHazusBuildingModel.h"
#include "RPopulationDisplacementEstimator.h"
#include "RDisasterPreparednessEstimator.h"
#include "RSutley2016MorbidityEstimator.h"


class RParameter;
class RResponse;

class RWaterDemandZoneModel : public RNetworkModel
{
    Q_OBJECT

    Q_PROPERTY(QObject *HouseholdPortfolio READ getHouseholdPortfolio WRITE setHouseholdPortfolio)
    Q_PROPERTY(QString PowerSupplierList READ getPowerSupplierList WRITE setPowerSupplierList)
    Q_PROPERTY(QString WaterSupplierList READ getWaterSupplierList WRITE setWaterSupplierList)

public:
    // Define the constructor and destructor
    RWaterDemandZoneModel(QObject *parent, QString name);
    ~RWaterDemandZoneModel();

    int resetTime();

    QObject* getHouseholdPortfolio() const;
    void setHouseholdPortfolio(QObject *value);

    QString getPowerSupplierList();
    void setPowerSupplierList(QString value);

    QString getWaterSupplierList();
    void setWaterSupplierList(QString value);

    void getRequests();

    // Define the method where the actual model is coded
    int evaluateModel();


    // Member functions called by the sampling model
    QVector<double> getPoorVector();
    QVector<double> getModerateCodeVector();
    QVector<double> getRichVector();
    QVector<double> getPreCodeVector();
    QVector<double> getLowCodeVector();
    QVector<double> getModCodeVector();
    QVector<double> getHighCodeVector();
    QVector<double> getOwnerVector();
    QVector<double> getRenterVector();
    QVector<double> getSFVector();
    QVector<double> getMFVector();
    QVector<double> getInsured();
    QVector<double> getNoInsured();
    QVector<double> getRecentVector();
    QVector<double> getEstablishedVector();
    QVector<double> getResidentVector();
    QVector<double> getFunctionalHabitability();
    QVector<double> getPerceivedHabitability();
    QVector<double> getMyResults();

private:
    // Intensity counters
    int theSa03Counter;
    int theSa1Counter;

    // Responses coming to this model
    QPointer<RHouseholdPortfolio> theHouseholdPortfolio = nullptr;
    QList<QPointer<RParameter>> thePowerSupplierList;
    QList<QPointer<RParameter>> theWaterSupplierList;

    // Utility networks
    bool theAvailabilityOfWater;
    bool theAvailabilityOfPower;
    double theWaterShortageDuration;
    double thePowerShortageDuration;

    QVector<double> thePoorVector;
    QVector<double> theModerateVector;
    QVector<double> theRichVector;
    QVector<double> thePreCodeVector;
    QVector<double> theLowCodeVector;
    QVector<double> theModCodeVector;
    QVector<double> theHighCodeVector;
    QVector<double> theOwnerVector;
    QVector<double> theRenterVector;
    QVector<double> theSFVector;
    QVector<double> theMFVector;
    QVector<double> theInsuredVector;
    QVector<double> theNotInsuredVector;
    QVector<double> theRecentVector;
    QVector<double> theEstablishedVector;
    QVector<double> theResidentVector;
    QVector<double> theFunctionalHabitabilityVector;
    QVector<double> thePerceivedHabitabilityVector;

    // Response
    RResponse* theResponse;
    RResponse* thePowerShortageDurationResponse;
    RResponse* theWaterShortageDurationResponse;

    // Random variables
    RContinuousRandomVariable * theta0;
    RContinuousRandomVariable * thetaLowIncome;
    RContinuousRandomVariable * thetaModerateIncome;
    RContinuousRandomVariable * thetaImmigration;
    RContinuousRandomVariable * thetaDisability;
    RContinuousRandomVariable * thetaHispanic;
    RContinuousRandomVariable * thetaHigherEd;

    double lowIncomeInCriticalNeedOfWater = 0.0;
    double modIncomeInCriticalNeedOfWater = 0.0;
    double highIncomeInCriticalNeedOfWater = 0.0;
    double hispanicInCriticalNeedOfWater = 0.0;
    double notHispanicInCriticalNeedOfWater = 0.0;
    double disabilityInCriticalNeedOfWater = 0.0;
    double noDisabilityInCriticalNeedOfWater = 0.0;
    double immigrantInCriticalNeedOfWater = 0.0;
    double notImmigrantInCriticalNeedOfWater = 0.0;
    double higherEdIncomeInCriticalNeedOfWater = 0.0;
    double noHigherEdIncomeInCriticalNeedOfWater = 0.0;

    QVector<double> myResults;
};

#endif // RWaterDemandZoneModel_H

#ifndef RSingleFamilyContractorFirmModel_H
#define RSingleFamilyContractorFirmModel_H

#include "RNetworkModel.h"
#include "RResponse.h"
#include "RModel.h"
#include "RPacket.h"
#include <QDateTime>

class RParameter;
class RResponse;

/*
 By Costa and Wang (2021)
 This class is used to simulate a contractor firm that is specialized on single-family buildings
 LocalCrews: number of construction crews existing in the community pre-disaster
 NewAccommodations: fraction [0,1] of the number of repaired buildings that can be used to house out-of-town workers
 DemandRatio: fraction [0,1] of the total demand for workers that is expected to be met at any time
 Behavior: behavior used by the Contractor Firm object to distributed workers when demand > supply
*/

class RSingleFamilyContractorFirmModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(int LocalCrews READ getLocalCrews WRITE setLocalCrews)
    Q_PROPERTY(double NewAccommodations READ getNewAccommodations WRITE setNewAccommodations)
    Q_PROPERTY(double DemandRatio READ getDemandRatio WRITE setDemandRatio)
    Q_PROPERTY(QString Behavior READ getBehavior WRITE setBehavior)

public:
    RSingleFamilyContractorFirmModel(QObject *parent, QString name);
    ~RSingleFamilyContractorFirmModel();

    int resetTime();

    int getLocalCrews();
    void setLocalCrews(int value);

    QString getBehavior();
    void setBehavior(QString value);

    double getDemandRatio();
    void setDemandRatio(double value);

    double getNewAccommodations();
    void setNewAccommodations(double value);

    void dismissTeamOfWorkers(QString theOccupancy, int value);

    void getRequests();

    int evaluateModel();

public slots:


private:   
    // Data members
    QVector<QVector<QString>> theBuildingOccupancy;
    QVector<double> theNeighborhoodIncome;
    QVector<QVector<double>> theBuildingAvailabilityOfFunds;

    // Vectors that track the amounts of workers (available, allocated, total supply)
    QVector<int> theAllocatedWorkersVector;
    QVector<int> theAvailableWorkersVector;
    QVector<int> theTotalDemandForWorkersVector;
    QVector<int> theNumberOfDwellingsWaitingVector;
    QVector<int> theRemainingAccomodationCapacityVector;

    QVector<QPointer<RModel>> thePacketDestinationList; // this is a copy of the destination list
    QVector<QVector<double>> thePacketAmountList; // this is a copy of the list of requests
    QVector<QString> thePacketSupplyTypeList;
    QVector<QVector<double>> thePacketBid;
    QList<QVector<int>> thePacketIDList;
    QVector<QVector<bool>> shouldGetWorkers;

    int theAvailableWorkersPreDisaster;
    int theNumberOfSingleFamilyCrews;
    int theNumberOfSingleFamilyWorkersWaitingToBeAllocated;
    int theNumberOfSingleFamilyWorkersCurrentlyAllocated;
    double theRemainingAccomodationCapacity;
    double theDemandRatio;
    double theSurplusWorkers;
    double theCurrentHourlyWages;
    double theInterventionApproachID;;
    QString theBehavior;

    // Remove after paper
    double samplingMultiplier = 0.0;

    double theTimeOfFirstRequest;
    double theTimeSinceLastUpdate;
    double theTimeOfEvent;

    // Responses
    QPointer<QCustomPlot> workforceHourlyCostCustomPlot;
    RResponse *theResponse;
};

#endif
